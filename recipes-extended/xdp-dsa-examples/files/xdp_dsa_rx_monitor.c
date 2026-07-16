// SPDX-License-Identifier: GPL-2.0
/*
 * DSA XDP RX Monitor Application
 *
 * This AF_XDP application demonstrates how to receive packets from
 * DSA switch ports and identify which port each packet came from.
 *
 * The application:
 * - Binds AF_XDP socket to conduit interface (e.g., eth0)
 * - Receives packets with the HMS DSA tag intact
 * - Parses the HMS DSA tag (a tag_8021q VLAN tag whose VID encodes the
 *   source switch port / switch id) to extract the source port
 * - Prints packet information per port
 *
 * This variant parses the DSA tag in userspace (the companion BPF
 * program xdp_dsa_redirect.o only redirects, it does not strip the
 * tag). For the metadata-based variant that offloads tag parsing and
 * stripping to the BPF program, see xdp_dsa_port_rx.c / xdp_dsa_meta.c.
 *
 * Usage: ./xdp_dsa_rx_monitor -i <conduit_interface> [-q <queue_id>] [-v]
 *
 * Example:
 *   ./xdp_dsa_rx_monitor -i eth0 -q 0 -v
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

/*
 * HMS DSA tag definitions (tag_8021q based).
 *
 * The HMS switch encodes the source port and switch id in the VID of a
 * VLAN tag (TPID 0x8100 C-tag or 0x88A8 S-tag). See net/dsa/tag_hms.c
 * and the VID bit layout in net/dsa/tag_8021q.c.
 */
#define HMS_VLAN_TPID_C     0x8100  /* ETH_P_8021Q  C-tag */
#define HMS_VLAN_TPID_S     0x88A8  /* ETH_P_HMS    S-tag (802.1AD) */
#define MAX_PORTS           16

/* tag_8021q VID bit layout: RSV[11:10]=3, SWITCH_ID[8:6], PORT[3:0]. */
#define HMS_VID_RSV_SHIFT       10
#define HMS_VID_RSV_MASK        0x03
#define HMS_VID_RSV_VAL         3
#define HMS_VID_SWITCH_SHIFT    6
#define HMS_VID_SWITCH_MASK     0x07
#define HMS_VID_PORT_SHIFT      0
#define HMS_VID_PORT_MASK       0x0F
#define HMS_TCI_VID_MASK        0x0FFF
#define HMS_TCI_PCP_SHIFT       13
#define HMS_TCI_PCP_MASK        0x07

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
#define XDP_DSA_OBJ_NAME    "xdp_dsa_redirect.o"

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


    /* Statistics */
    struct port_stats ports[MAX_PORTS];
    __u64 unknown_packets;
};

static volatile int running = 1;

static void signal_handler(int sig)
{
    (void)sig;
    running = 0;
}

/*
 * Parse the HMS DSA tag (tag_8021q VLAN tag) from a received packet.
 *
 * The source port and switch id are encoded in the VLAN VID. Only
 * genuine DSA tag_8021q VIDs (RSV bits == 3) are accepted.
 *
 * Returns: 0 on success with port/switch_id filled, -1 if the frame is
 * not an HMS DSA tagged data frame.
 */
static int parse_hms_tag(void *pkt, size_t len, __u8 *port, __u8 *switch_id)
{
    struct ethhdr *eth = pkt;
    __be16 *tci_p;
    __u16 tpid, tci, vid;

    /* Need at least Ethernet header + VLAN TCI */
    if (len < sizeof(struct ethhdr) + sizeof(__be16))
        return -1;

    /* Check for an HMS VLAN tag (C-tag or S-tag) */
    tpid = ntohs(eth->h_proto);
    if (tpid != HMS_VLAN_TPID_C && tpid != HMS_VLAN_TPID_S)
        return -1;

    /*
     * The VLAN TCI sits right after the Ethernet header (the TPID is
     * carried in eth->h_proto). Read the TCI and decode the VID.
     */
    tci_p = (__be16 *)(eth + 1);
    tci = ntohs(*tci_p);
    vid = tci & HMS_TCI_VID_MASK;

    /* Only act on genuine DSA tag_8021q VIDs (RSV bits == 3). */
    if (((vid >> HMS_VID_RSV_SHIFT) & HMS_VID_RSV_MASK) != HMS_VID_RSV_VAL)
        return -1;

    *port = (vid >> HMS_VID_PORT_SHIFT) & HMS_VID_PORT_MASK;
    *switch_id = (vid >> HMS_VID_SWITCH_SHIFT) & HMS_VID_SWITCH_MASK;

    return 0;
}

/*
 * Print packet information
 */
static void print_packet_info(struct xdp_app *app, void *pkt, size_t len,
                              __u8 port, __u8 switch_id)
{
    struct ethhdr *eth = pkt;
    __u16 inner_proto = 0;
    struct timespec ts;
    char time_str[32];

    /* Get timestamp */
    clock_gettime(CLOCK_REALTIME, &ts);
    strftime(time_str, sizeof(time_str), "%H:%M:%S", localtime(&ts.tv_sec));

    if (len >= (size_t)(ETH_HLEN + 2 + sizeof(__be16)))
        inner_proto = ntohs(*(__be16 *)(pkt + ETH_HLEN + 2));

    printf("[%s.%06ld] hms0p%u (switch %u): ",
           time_str, ts.tv_nsec / 1000, port, switch_id);
    printf("%02x:%02x:%02x:%02x:%02x:%02x -> %02x:%02x:%02x:%02x:%02x:%02x ",
           eth->h_source[0], eth->h_source[1], eth->h_source[2],
           eth->h_source[3], eth->h_source[4], eth->h_source[5],
           eth->h_dest[0], eth->h_dest[1], eth->h_dest[2],
           eth->h_dest[3], eth->h_dest[4], eth->h_dest[5]);
    printf("proto=0x%04x len=%zu\n", inner_proto, len);

    if (app->verbose) {
        /* Print hex dump of first 64 bytes */
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
 * Process received packets
 */
static void process_rx(struct xdp_app *app)
{
    unsigned int rcvd, i;
    __u32 idx_rx = 0;
    __u32 idx_fq = 0;

    /* Peek available RX descriptors */
    rcvd = xsk_ring_cons__peek(&app->rx, RX_BATCH_SIZE, &idx_rx);
    if (!rcvd)
        return;

    /* Reserve space in fill queue for returning buffers */
    if (xsk_ring_prod__reserve(&app->fq, rcvd, &idx_fq) != rcvd) {
        /* Not enough space in fill queue - try anyway */
        fprintf(stderr, "Warning: Fill queue full\n");
    }

    for (i = 0; i < rcvd; i++) {
        const struct xdp_desc *desc;
        void *pkt;
        size_t len;
        __u8 port = 0;
        __u8 switch_id = 0;
        __u64 addr;

        desc = xsk_ring_cons__rx_desc(&app->rx, idx_rx + i);
        addr = desc->addr;
        len = desc->len;
        pkt = xsk_umem__get_data(app->umem_area, addr);

        /* Parse DSA tag to get source port */
        if (parse_hms_tag(pkt, len, &port, &switch_id) == 0) {
            /* Update per-port statistics */
            if (port < MAX_PORTS) {
                app->ports[port].rx_packets++;
                app->ports[port].rx_bytes += len;
            }

            /* Print packet info */
            print_packet_info(app, pkt, len, port, switch_id);
        } else {
            app->unknown_packets++;
            if (app->verbose) {
                printf("[Unknown] Non-HMS-tagged packet, len=%zu, "
                       "ethertype=0x%04x\n",
                       len, ntohs(((struct ethhdr *)pkt)->h_proto));
            }
        }

        /* Return buffer to fill queue */
        *xsk_ring_prod__fill_addr(&app->fq, idx_fq + i) = addr;
    }

    /* Release consumed RX descriptors */
    xsk_ring_cons__release(&app->rx, rcvd);

    /* Submit returned buffers to fill queue */
    xsk_ring_prod__submit(&app->fq, rcvd);
}

/*
 * Print final statistics
 */
static void print_statistics(struct xdp_app *app)
{
    printf("\n=== RX Statistics ===\n");
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

    if (app->unknown_packets > 0) {
        printf("%-10s  %15llu  %15s\n", "Unknown",
               (unsigned long long)app->unknown_packets, "N/A");
    }

    printf("=====================\n");
}

/*
 * Setup UMEM (shared memory for packet buffers)
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

    /* Allocate aligned memory for UMEM */
    ret = posix_memalign(&app->umem_area, getpagesize(), umem_size);
    if (ret) {
        fprintf(stderr, "Failed to allocate UMEM: %s\n", strerror(ret));
        return -ret;
    }

    /* Create UMEM */
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
 * Populate fill queue with initial buffers
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

    for (__u32 i = 0; i < FILL_RING_SIZE; i++) {
        *xsk_ring_prod__fill_addr(&app->fq, idx + i) = i * FRAME_SIZE;
    }

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
 * Load and attach XDP program
 */
static int load_xdp_program(struct xdp_app *app)
{
    struct bpf_map *xsk_map;
    char obj_path[PATH_MAX];
    int ret;

    /* Load XDP program from object file */
    resolve_obj_path(obj_path, sizeof(obj_path), XDP_DSA_OBJ_NAME);
    app->xdp_prog = xdp_program__open_file(obj_path, "xdp", NULL);

    if (libxdp_get_error(app->xdp_prog)) {
        fprintf(stderr, "Failed to load XDP program: %s\n",
                strerror(libxdp_get_error(app->xdp_prog)));
        return -1;
    }

    /* Attach XDP program to interface */
    ret = xdp_program__attach(app->xdp_prog, app->ifindex, app->xdp_mode, 0);
    if (ret) {
        fprintf(stderr, "Failed to attach XDP program: %s\n", strerror(-ret));
        xdp_program__close(app->xdp_prog);
        return ret;
    }

    /* Get XSK map fd */
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
 * Create XSK socket
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
        .bind_flags = XDP_USE_NEED_WAKEUP,
    };
    int ret;

    ret = xsk_socket__create(&app->xsk, app->ifname, app->queue_id,
                             app->umem, &app->rx, NULL, &cfg);
    if (ret) {
        fprintf(stderr, "Failed to create XSK socket: %s\n", strerror(-ret));
        return ret;
    }

    /* Update XSK map with socket fd */
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
 * Cleanup resources
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
        "DSA XDP RX Monitor - Monitor packets on DSA switch ports\n"
        "\n"
        "Options:\n"
        "  -i <ifname>   Conduit interface name (e.g., eth0) [required]\n"
        "  -q <queue>    RX queue ID (default: 0)\n"
        "  -v            Verbose output (hex dump)\n"
        "  -h            Show this help\n"
        "\n"
        "Example:\n"
        "  %s -i eth0 -q 0 -v\n"
        "\n"
        "Note: This application binds to the conduit interface (eth0),\n"
        "      not the DSA user ports (hms0pX). The DSA tag is parsed to\n"
        "      identify which switch port the packet came from.\n",
        prog, prog);
}

int main(int argc, char **argv)
{
    struct xdp_app app = {0};
    struct pollfd fds[1];
    int opt, ret;

    /* Default to native driver XDP mode. */
    app.xdp_mode = XDP_MODE_NATIVE;

    /* Parse command line arguments */
    while ((opt = getopt(argc, argv, "i:q:vh")) != -1) {
        switch (opt) {
        case 'i':
            strncpy(app.ifname, optarg, IFNAMSIZ - 1);
            break;
        case 'q':
            app.queue_id = atoi(optarg);
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

    /* Get interface index */
    app.ifindex = if_nametoindex(app.ifname);
    if (!app.ifindex) {
        fprintf(stderr, "Error: Interface %s not found\n", app.ifname);
        return 1;
    }

    /* Setup signal handlers */
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);

    printf("DSA XDP RX Monitor\n");
    printf("==================\n");
    printf("Interface: %s (index %d)\n", app.ifname, app.ifindex);
    printf("Queue: %u\n", app.queue_id);
    printf("Press Ctrl+C to stop\n\n");

    /* Setup UMEM */
    ret = setup_umem(&app);
    if (ret)
        goto out;

    /* Load XDP program */
    ret = load_xdp_program(&app);
    if (ret)
        goto out;

    /* Create XSK socket */
    ret = create_xsk_socket(&app);
    if (ret)
        goto out;

    /* Populate fill queue */
    ret = populate_fill_queue(&app);
    if (ret)
        goto out;

    printf("\nMonitoring packets...\n\n");

    /* Main receive loop */
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
