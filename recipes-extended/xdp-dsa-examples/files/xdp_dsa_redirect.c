// SPDX-License-Identifier: GPL-2.0
/*
 * DSA XDP Redirect Program (HMS switch)
 *
 * This XDP program runs on the HMS DSA conduit interface and redirects
 * data-path frames to an AF_XDP socket bound to the conduit's RX queue.
 * Unlike xdp_dsa_meta.c, this program does NOT strip the DSA tag: the
 * HMS tag_8021q VLAN tag is left intact so the userspace application
 * (xdp_dsa_rx_monitor) can parse it to identify the source switch port.
 *
 * Control-plane frames are deliberately left for the kernel DSA stack
 * (XDP_PASS), so that switch management, bridging and PTP keep working:
 *
 *   - HMS in-band control / timestamp frames (ethertype 0xDADC,
 *     ETH_P_HMS_META) consumed by net/dsa/tag_hms.c.
 *   - PTP and slow-protocol link-local frames (DMAC 01:1B:19:xx:xx:xx
 *     and 01:80:C2:xx:xx:xx) that the switch traps to the host.
 *   - Untagged frames and real (non-DSA) VLAN frames whose VID is not a
 *     tag_8021q VID (RSV bits != 3).
 *
 * Copyright 2026 NXP
 */

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <bpf/bpf_helpers.h>
#include <bpf/bpf_endian.h>

#include "xdp_dsa_meta.h"

/* XSK map for AF_XDP sockets, indexed by RX queue id. */
struct {
    __uint(type, BPF_MAP_TYPE_XSKMAP);
    __uint(max_entries, 8);
    __type(key, __u32);
    __type(value, __u32);
} xsk_map SEC(".maps");

/* IEEE 802.3 slow protocols: 01:80:C2:xx:xx:xx (see hms.h FILTER_A). */
#define HMS_LL_SLOW_HI		0x0180
#define HMS_LL_SLOW_MID		0xC200
/* IEEE 1588 PTP over Ethernet: 01:1B:19:xx:xx:xx (see hms.h FILTER_B). */
#define HMS_LL_PTP_HI		0x011B
#define HMS_LL_PTP_MID		0x1900

/*
 * Return true if the destination MAC is an HMS link-local address that
 * must stay in the kernel stack (PTP or slow protocols). Mirrors
 * hms_is_link_local() in net/dsa/tag_hms.c, which matches on the top
 * 24 bits of the DMAC.
 */
static __always_inline int hms_is_link_local(const struct ethhdr *eth)
{
    __u16 hi = bpf_ntohs(*(const __be16 *)&eth->h_dest[0]);
    __u16 mid = bpf_ntohs(*(const __be16 *)&eth->h_dest[2]);

    if (hi == HMS_LL_SLOW_HI && mid == HMS_LL_SLOW_MID)
        return 1;
    if (hi == HMS_LL_PTP_HI && mid == HMS_LL_PTP_MID)
        return 1;

    return 0;
}

SEC("xdp")
int xdp_redirect_xsk(struct xdp_md *ctx)
{
    void *data = (void *)(long)ctx->data;
    void *data_end = (void *)(long)ctx->data_end;
    struct ethhdr *eth = data;
    __u32 queue_id = ctx->rx_queue_index;
    __be16 *tci_p;
    __u16 ethproto, tci, vid;

    /* Bounds check: Ethernet header. */
    if ((void *)(eth + 1) > data_end)
        return XDP_PASS;

    ethproto = bpf_ntohs(eth->h_proto);

    /*
     * Leave the control plane to the kernel DSA stack:
     *   - HMS meta / timestamp frames (ETH_P_HMS_META).
     *   - PTP / slow-protocol link-local frames.
     * These keep switch management and PTP working as usual.
     */
    if (ethproto == HMS_META_ETHERTYPE)
        return XDP_PASS;
    if (hms_is_link_local(eth))
        return XDP_PASS;

    /* Data path: HMS uses a tag_8021q VLAN tag (C-tag or S-tag). */
    if (ethproto != HMS_VLAN_TPID_C && ethproto != HMS_VLAN_TPID_S)
        return XDP_PASS;

    /*
     * The VLAN TCI sits right after the Ethernet header (the TPID is
     * carried in eth->h_proto). Read it and decode the VID.
     */
    tci_p = (void *)(eth + 1);
    if ((void *)(tci_p + 1) > data_end)
        return XDP_PASS;

    tci = bpf_ntohs(*tci_p);
    vid = tci & HMS_TCI_VID_MASK;

    /*
     * Only act on genuine DSA tag_8021q VIDs (RSV bits == 3). A real
     * customer VLAN that happens to use 0x8100 is left untouched for
     * the kernel stack.
     */
    if (((vid >> HMS_VID_RSV_SHIFT) & HMS_VID_RSV_MASK) != HMS_VID_RSV_VAL)
        return XDP_PASS;

    /*
     * Redirect the DSA-tagged frame to the AF_XDP socket bound to this
     * RX queue. The DSA tag is preserved for userspace parsing. If no
     * socket is bound to the queue, fall back to XDP_PASS so the frame
     * reaches the normal network stack.
     */
    return bpf_redirect_map(&xsk_map, queue_id, XDP_PASS);
}

char _license[] SEC("license") = "GPL";
