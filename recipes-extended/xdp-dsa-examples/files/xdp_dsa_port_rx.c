// SPDX-License-Identifier: GPL-2.0
/*
 * DSA XDP Port RX Application (metadata-based)
 *
 * This AF_XDP application demonstrates the metadata-based DSA receive
 * path. It is the userspace counterpart of xdp_dsa_meta.c.
 *
 * Unlike xdp_dsa_rx_monitor.c (which re-parses the HMS DSA tag in
 * userspace), this application relies on the BPF program to:
 *   - parse the HMS DSA tag,
 *   - record the source switch port / switch ID / IPV into the XDP
 *     metadata area (struct xdp_dsa_meta), and
 *   - strip the DSA tag before redirecting to the AF_XDP socket.
 *
 * As a result, each frame delivered here starts at a clean Ethernet
 * header (no DSA tag), and the source port is read from the metadata
 * region that sits immediately in front of the packet data in the
 * UMEM frame.
 *
 * Metadata layout in the UMEM frame:
 *
 *   addr-meta_len            addr (desc->addr)
 *   |                        |
 *   v                        v
 *   +----------------------+ +-------------------------------+
 *   | struct xdp_dsa_meta  | | Ethernet header + payload     |
 *   +----------------------+ +-------------------------------+
 *
 * The metadata length is reported per descriptor via the
 * XDP_RX_METADATA option (xsk_umem__has_metadata path) when the
 * kernel/driver supports it. For portability this example reads the
 * metadata from the fixed negative offset in front of desc->addr,
 * which is how AF_XDP delivers bpf_xdp_adjust_meta() data.
 *
 * Usage: ./xdp_dsa_port_rx -i <conduit_interface> [-q <queue_id>] [-v]
 *
 * Example:
 *   ./xdp_dsa_port_rx -i eth0 -q 0 -v
 *
 * Copyright 2026 NXP
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <getopt.h>
#include <time.h>
#include <poll.h>
#include <limits.h>
#include <net/if.h>

#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <linux/if_link.h>
#include <linux/if_xdp.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>
#include <xdp/xsk.h>
#include <xdp/libxdp.h>

#include "xdp_dsa_meta.h"

/* Application limits */
#define MAX_PORTS           32

/* XSK configuration */
#define NUM_FRAMES          4096
#define FRAME_SIZE          XSK_UMEM__DEFAULT_FRAME_SIZE
#define RX_BATCH_SIZE       64
#define FILL_RING_SIZE      XSK_RING_PROD__DEFAULT_NUM_DESCS
#define COMP_RING_SIZE      XSK_RING_CONS__DEFAULT_NUM_DESCS

/*
 * BPF object file lookup.
 *
 * xdp_program__open_file() treats its filename argument as a literal
 * path: a bare name is resolved against the current working directory,
 * not against the directory of this binary or any install directory.
 * To let the application run from any working directory, the BPF object
 * is loaded from a fixed install directory.
 *
 * XDP_DSA_OBJDIR is the compile-time default install directory, set by
 * the build system (-DXDP_DSA_OBJDIR=...). It can be overridden at run
 * time with the XDP_DSA_OBJDIR environment variable, which is useful
 * when running the examples from a build tree.
 */
#ifndef XDP_DSA_OBJDIR
#define XDP_DSA_OBJDIR      "/usr/share/xdp-dsa-examples"
#endif
#define XDP_DSA_OBJ_ENVVAR  "XDP_DSA_OBJDIR"

/* Name of the BPF object file this application loads. */
#define XDP_DSA_OBJ_NAME    "xdp_dsa_meta.o"


/* Per-port statistics */
struct port_stats {
    __u64 rx_packets;
    __u64 rx_bytes;
};

/* Application context */
struct xdp_app {
    /* XSK resources */
    struct xsk_socket *xsk;
    struct xsk_ring_cons rx;
    struct xsk_ring_prod fq;
    struct xsk_ring_cons cq;
    struct xsk_umem *umem;
    void *umem_area;

    /* XDP program */
    struct xdp_program *xdp_prog;
    int xsk_map_fd;

    /* Configuration */
    char ifname[IFNAMSIZ];
    int ifindex;
    __u32 queue_id;
    int verbose;
    int xdp_mode;       /* XDP_MODE_NATIVE (default) or XDP_MODE_SKB */
    __u16 bind_flags;   /* extra bind flags: XDP_ZEROCOPY / XDP_COPY */


    /* Statistics */
    struct port_stats ports[MAX_PORTS];
    __u64 no_meta_packets;
    __u64 invalid_meta_packets;
};

static volatile int running = 1;

static void signal_handler(int sig)
{
    (void)sig;
    running = 0;
}

/*
 * Read the XDP metadata that sits just in front of the packet data.
 *
 * The BPF program reserved sizeof(struct xdp_dsa_meta) bytes via
 * bpf_xdp_adjust_meta(). AF_XDP delivers that region immediately
 * before the data pointed to by desc->addr. We therefore look back
 * one struct's worth of bytes from the packet start.
 *
 * Returns a pointer to the metadata, or NULL if the negative offset
 * would fall outside the UMEM frame (e.g. a frame without metadata).
 */
static const struct xdp_dsa_meta *get_meta(struct xdp_app *app, __u64 addr)
{
    /* Frame-aligned base of the UMEM frame holding this packet. */
    __u64 frame_base = addr & ~((__u64)FRAME_SIZE - 1);

    if (addr < frame_base + sizeof(struct xdp_dsa_meta))
        return NULL;

    return (const struct xdp_dsa_meta *)
        xsk_umem__get_data(app->umem_area, addr - sizeof(struct xdp_dsa_meta));
}

/*
 * Print packet information using the metadata-provided port.
 */
static void print_packet_info(struct xdp_app *app, void *pkt, size_t len,
                              const struct xdp_dsa_meta *meta)
{
    struct ethhdr *eth = pkt;
    struct timespec ts;
    char time_str[32];

    clock_gettime(CLOCK_REALTIME, &ts);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&ts.tv_sec));

    printf("[%s.%06ld] hms0p%u (switch %u, ipv %u): ",
           time_str, ts.tv_nsec / 1000,
           meta->src_port, meta->switch_id, meta->ipv);
    printf("%02x:%02x:%02x:%02x:%02x:%02x -> %02x:%02x:%02x:%02x:%02x:%02x ",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5],
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("proto=0x%04x len=%zu%s\n",
           ntohs(eth->h_proto), len,
           (meta->flags & XDP_DSA_META_F_TAG_STRIPPED) ? " [tag-stripped]" : "");

    if (app->verbose) {
        printf("  Hex: ");
        for (size_t i = 0; i < (len < 64 ? len : 64); i++) {
            printf("%02x ", ((unsigned char *)pkt)[i]);
            if ((i + 1) % 16 == 0 && i + 1 < 64)
                printf("\n       ");
        }
        printf("\n");
    }
}

/*
 * Process received packets.
 */
static void process_rx(struct xdp_app *app)
{
    unsigned int rcvd, i;
    __u32 idx_rx = 0;
    __u32 idx_fq = 0;

    rcvd = xsk_ring_cons__peek(&app->rx, RX_BATCH_SIZE, &idx_rx);
    if (!rcvd)
        return;

    if (xsk_ring_prod__reserve(&app->fq, rcvd, &idx_fq) != rcvd)
        fprintf(stderr, "Warning: Fill queue full\n");

    for (i = 0; i < rcvd; i++) {
        const struct xdp_desc *desc;
        const struct xdp_dsa_meta *meta;
        void *pkt;
        size_t len;
        __u64 addr;

        desc = xsk_ring_cons__rx_desc(&app->rx, idx_rx + i);
        addr = desc->addr;
        len = desc->len;
        pkt = xsk_umem__get_data(app->umem_area, addr);

        meta = get_meta(app, addr);
        if (!meta) {
            app->no_meta_packets++;
            if (app->verbose)
                printf("[No-meta] frame without metadata, len=%zu\n", len);
        } else if (!(meta->flags & XDP_DSA_META_F_VALID)) {
            app->invalid_meta_packets++;
            if (app->verbose)
                printf("[Invalid-meta] flags=0x%02x len=%zu\n",
                       meta->flags, len);
        } else {
            if (meta->src_port < MAX_PORTS) {
                app->ports[meta->src_port].rx_packets++;
                app->ports[meta->src_port].rx_bytes += len;
            }
            print_packet_info(app, pkt, len, meta);
        }

        /* Return buffer to fill queue. */
        *xsk_ring_prod__fill_addr(&app->fq, idx_fq + i) = addr;
    }

    xsk_ring_cons__release(&app->rx, rcvd);
    xsk_ring_prod__submit(&app->fq, rcvd);
}

/*
 * Print final statistics.
 */
static void print_statistics(struct xdp_app *app)
{
    printf("\n=== RX Statistics (metadata-based) ===\n");
    printf("%-10s  %15s  %15s\n", "Port", "Packets", "Bytes");
    printf("%-10s  %15s  %15s\n", "----", "-------", "-----");

    for (int i = 0; i < MAX_PORTS; i++) {
        if (app->ports[i].rx_packets > 0) {
            char portname[16];

            snprintf(portname, sizeof(portname), "hms0p%d", i);
            printf("%-10s  %15llu  %15llu\n",
                   portname,
                   (unsigned long long)app->ports[i].rx_packets,
                   (unsigned long long)app->ports[i].rx_bytes);
        }
    }

    if (app->no_meta_packets > 0)
        printf("%-10s  %15llu  %15s\n", "No-meta",
               (unsigned long long)app->no_meta_packets, "N/A");
    if (app->invalid_meta_packets > 0)
        printf("%-10s  %15llu  %15s\n", "Invalid",
               (unsigned long long)app->invalid_meta_packets, "N/A");

    printf("======================================\n");
}

/*
 * Setup UMEM (shared memory for packet buffers).
 */
static int setup_umem(struct xdp_app *app)
{
    struct xsk_umem_config cfg = {
        .fill_size = FILL_RING_SIZE,
        .comp_size = COMP_RING_SIZE,
        .frame_size = FRAME_SIZE,
        .frame_headroom = XSK_UMEM__DEFAULT_FRAME_HEADROOM,
        .flags = 0
    };
    size_t umem_size = NUM_FRAMES * FRAME_SIZE;
    int ret;

    ret = posix_memalign(&app->umem_area, getpagesize(), umem_size);
    if (ret) {
        fprintf(stderr, "Failed to allocate UMEM: %s\n", strerror(ret));
        return -ret;
    }

    ret = xsk_umem__create(&app->umem, app->umem_area, umem_size,
                           &app->fq, &app->cq, &cfg);
    if (ret) {
        fprintf(stderr, "Failed to create UMEM: %s\n", strerror(-ret));
        free(app->umem_area);
        return ret;
    }

    return 0;
}

/*
 * Populate fill queue with initial buffers.
 */
static int populate_fill_queue(struct xdp_app *app)
{
    __u32 idx;
    int ret;

    ret = xsk_ring_prod__reserve(&app->fq, FILL_RING_SIZE, &idx);
    if (ret != FILL_RING_SIZE) {
        fprintf(stderr, "Failed to reserve fill queue\n");
        return -ENOMEM;
    }

    for (__u32 i = 0; i < FILL_RING_SIZE; i++)
        *xsk_ring_prod__fill_addr(&app->fq, idx + i) = i * FRAME_SIZE;

    xsk_ring_prod__submit(&app->fq, FILL_RING_SIZE);

    return 0;
}

/*
 * Resolve the full path to a BPF object file.
 *
 * The directory is taken from the XDP_DSA_OBJDIR environment variable
 * when set, otherwise the compile-time default XDP_DSA_OBJDIR. The
 * resulting absolute path is what gets passed to
 * xdp_program__open_file(), so the application no longer depends on the
 * current working directory.
 */
static const char *resolve_obj_path(char *buf, size_t buflen, const char *name)
{
    const char *dir = getenv(XDP_DSA_OBJ_ENVVAR);

    if (!dir || dir[0] == '\0')
        dir = XDP_DSA_OBJDIR;

    snprintf(buf, buflen, "%s/%s", dir, name);
    return buf;
}

/*
 * Load and attach the metadata XDP program.
 */
static int load_xdp_program(struct xdp_app *app)
{
    struct bpf_map *xsk_map;
    char obj_path[PATH_MAX];
    int ret;

    resolve_obj_path(obj_path, sizeof(obj_path), XDP_DSA_OBJ_NAME);
    app->xdp_prog = xdp_program__open_file(obj_path, "xdp", NULL);

    if (libxdp_get_error(app->xdp_prog)) {
        fprintf(stderr, "Failed to load XDP program: %s\n",
                strerror(libxdp_get_error(app->xdp_prog)));
        return -1;
    }

    ret = xdp_program__attach(app->xdp_prog, app->ifindex, app->xdp_mode, 0);
    if (ret) {
        fprintf(stderr, "Failed to attach XDP program: %s\n", strerror(-ret));
        xdp_program__close(app->xdp_prog);
        return ret;
    }

    xsk_map = bpf_object__find_map_by_name(
        xdp_program__bpf_obj(app->xdp_prog), "xsk_map");
    if (!xsk_map) {
        fprintf(stderr, "Failed to find xsk_map\n");
        xdp_program__detach(app->xdp_prog, app->ifindex, app->xdp_mode, 0);
        xdp_program__close(app->xdp_prog);
        return -1;
    }


    app->xsk_map_fd = bpf_map__fd(xsk_map);

    printf("XDP program loaded and attached to %s\n", app->ifname);
    return 0;
}

/*
 * Create XSK socket.
 */
static int create_xsk_socket(struct xdp_app *app)
{
    struct xsk_socket_config cfg = {
        .rx_size = XSK_RING_CONS__DEFAULT_NUM_DESCS,
        .tx_size = 0,  /* RX only - no TX ring */
        /*
         * Let libxdp manage the XDP program we already attached rather
         * than loading its built-in dispatcher.
         */
        .libbpf_flags = XSK_LIBBPF_FLAGS__INHIBIT_PROG_LOAD,
        .xdp_flags = 0,
        /*
         * bind_flags controls the AF_XDP data path:
         *   - XDP_ZEROCOPY : require the driver zero-copy path; bind
         *     fails if the driver/queue cannot provide it.
         *   - XDP_COPY     : force the generic copy path.
         *   - 0 (neither)  : kernel auto-negotiates, preferring
         *     zero-copy when the driver supports it and silently
         *     falling back to copy otherwise.
         * XDP_USE_NEED_WAKEUP is always set so the driver can ask us to
         * kick the fill ring, which both modes benefit from.
         */
        .bind_flags = XDP_USE_NEED_WAKEUP | app->bind_flags,
    };
    int ret;

    ret = xsk_socket__create(&app->xsk, app->ifname, app->queue_id,
                             app->umem, &app->rx, NULL, &cfg);
    if (ret) {
        fprintf(stderr, "Failed to create XSK socket: %s\n", strerror(-ret));
        if (ret == -EOPNOTSUPP && (app->bind_flags & XDP_ZEROCOPY))
            fprintf(stderr,
                    "Hint: driver/queue does not support zero-copy on this "
                    "interface; retry without -z (auto) or with -c (copy).\n");
        return ret;
    }


    ret = bpf_map_update_elem(app->xsk_map_fd, &app->queue_id,
                              &(int){xsk_socket__fd(app->xsk)}, 0);
    if (ret) {
        fprintf(stderr, "Failed to update xsk_map: %s\n", strerror(-ret));
        xsk_socket__delete(app->xsk);
        return ret;
    }

    printf("XSK socket created on %s queue %u\n", app->ifname, app->queue_id);
    return 0;
}

/*
 * Cleanup resources.
 */
static void cleanup(struct xdp_app *app)
{
    if (app->xsk)
        xsk_socket__delete(app->xsk);

    if (app->umem)
        xsk_umem__delete(app->umem);

    if (app->umem_area)
        free(app->umem_area);

    if (app->xdp_prog) {
        xdp_program__detach(app->xdp_prog, app->ifindex, app->xdp_mode, 0);
        xdp_program__close(app->xdp_prog);
    }
}


static void usage(const char *prog)
{
    fprintf(stderr,
        "Usage: %s -i <interface> [-q <queue_id>] [-v]\n"
        "\n"
        "DSA XDP Port RX - Identify DSA switch ports via XDP metadata\n"
        "\n"
        "Options:\n"
        "  -i <ifname>   Conduit interface name (e.g., eth0) [required]\n"
        "  -q <queue>    RX queue ID (default: 0)\n"
        "  -z            Require zero-copy (XDP_ZEROCOPY); fail if unsupported\n"
        "  -c            Force copy mode (XDP_COPY)\n"
        "  -S            Use SKB/generic XDP mode (default: native driver mode)\n"
        "  -v            Verbose output (hex dump)\n"
        "  -h            Show this help\n"
        "\n"
        "Data path: with neither -z nor -c the kernel auto-negotiates and\n"
        "prefers zero-copy when the driver supports it (e.g. ENETC), and\n"
        "silently falls back to copy mode otherwise.\n"
        "\n"
        "Example:\n"
        "  %s -i eth0 -q 0 -z -v\n"

        "\n"
        "Note: This application binds to the conduit interface (eth0),\n"
        "      not the DSA user ports (hms0pX). The companion BPF program\n"
        "      xdp_dsa_meta.o parses the DSA tag, records the source port\n"
        "      in the XDP metadata area, and strips the tag, so frames\n"
        "      arrive here as clean Ethernet frames.\n",
        prog, prog);
}

int main(int argc, char **argv)
{
    struct xdp_app app = {0};
    struct pollfd fds[1];
    int opt, ret;

    /* Defaults: native driver XDP, kernel auto-negotiated data path. */
    app.xdp_mode = XDP_MODE_NATIVE;
    app.bind_flags = 0;

    while ((opt = getopt(argc, argv, "i:q:zcSvh")) != -1) {
        switch (opt) {
        case 'i':
            strncpy(app.ifname, optarg, IFNAMSIZ - 1);
            break;
        case 'q':
            app.queue_id = atoi(optarg);
            break;
        case 'z':
            app.bind_flags |= XDP_ZEROCOPY;
            break;
        case 'c':
            app.bind_flags |= XDP_COPY;
            break;
        case 'S':
            app.xdp_mode = XDP_MODE_SKB;
            break;
        case 'v':
            app.verbose = 1;
            break;
        case 'h':
        default:
            usage(argv[0]);
            return opt == 'h' ? 0 : 1;
        }
    }

    if (strlen(app.ifname) == 0) {
        fprintf(stderr, "Error: Interface name required\n");
        usage(argv[0]);
        return 1;
    }

    if ((app.bind_flags & XDP_ZEROCOPY) && (app.bind_flags & XDP_COPY)) {
        fprintf(stderr, "Error: -z and -c are mutually exclusive\n");
        return 1;
    }

    /*
     * Zero-copy needs a driver-native XDP data path; SKB/generic mode
     * is always copy. Reject the contradictory combination early.
     */
    if ((app.bind_flags & XDP_ZEROCOPY) && app.xdp_mode == XDP_MODE_SKB) {
        fprintf(stderr, "Error: zero-copy (-z) requires native mode (drop -S)\n");
        return 1;
    }


    app.ifindex = if_nametoindex(app.ifname);
    if (!app.ifindex) {
        fprintf(stderr, "Error: Interface %s not found\n", app.ifname);
        return 1;
    }

    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("DSA XDP Port RX (metadata-based)\n");
    printf("================================\n");
    printf("Interface: %s (index %d)\n", app.ifname, app.ifindex);
    printf("Queue: %u\n", app.queue_id);
    printf("XDP mode: %s\n",
           app.xdp_mode == XDP_MODE_SKB ? "SKB/generic" : "native");
    printf("Data path: %s\n",
           (app.bind_flags & XDP_ZEROCOPY) ? "zero-copy (required)" :
           (app.bind_flags & XDP_COPY)     ? "copy (forced)" :
                                             "auto (zero-copy preferred)");
    printf("Press Ctrl+C to stop\n\n");


    ret = setup_umem(&app);
    if (ret)
        goto out;

    ret = load_xdp_program(&app);
    if (ret)
        goto out;

    ret = create_xsk_socket(&app);
    if (ret)
        goto out;

    ret = populate_fill_queue(&app);
    if (ret)
        goto out;

    printf("\nMonitoring packets...\n\n");

    fds[0].fd = xsk_socket__fd(app.xsk);
    fds[0].events = POLLIN;

    while (running) {
        ret = poll(fds, 1, 1000);  /* 1 second timeout */
        if (ret < 0) {
            if (errno == EINTR)
                continue;
            fprintf(stderr, "Poll error: %s\n", strerror(errno));
            break;
        }

        if (ret > 0 && (fds[0].revents & POLLIN))
            process_rx(&app);
    }

    print_statistics(&app);

out:
    cleanup(&app);
    return ret ? 1 : 0;
}
