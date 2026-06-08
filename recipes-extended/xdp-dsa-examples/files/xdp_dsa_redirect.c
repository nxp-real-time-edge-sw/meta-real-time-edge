// SPDX-License-Identifier: GPL-2.0
/*
 * DSA XDP Redirect Program
 *
 * This XDP program redirects all packets to an AF_XDP socket.
 * The DSA tag is preserved in the packet for userspace parsing.
 *
 * Copyright 2026 NXP
 */

#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

/* XSK map for AF_XDP sockets */
struct {
    __uint(type, BPF_MAP_TYPE_XSKMAP);
    __uint(max_entries, 4);  /* Number of queues */
    __type(key, __u32);
    __type(value, __u32);
} xsk_map SEC(".maps");

SEC("xdp")
int xdp_redirect_xsk(struct xdp_md *ctx)
{
    __u32 queue_id = ctx->rx_queue_index;

    /*
     * Redirect all packets to AF_XDP socket.
     * The packet still contains the DSA tag, which will be
     * parsed by the userspace application to identify the
     * source switch port.
     *
     * If no AF_XDP socket is bound to this queue, fall back
     * to XDP_PASS to deliver packet to normal network stack.
     */
    return bpf_redirect_map(&xsk_map, queue_id, XDP_PASS);
}

char _license[] SEC("license") = "GPL";
