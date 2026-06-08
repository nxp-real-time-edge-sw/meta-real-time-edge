// SPDX-License-Identifier: GPL-2.0
/*
 * DSA XDP metadata + tag-strip + redirect program (HMS switch)
 *
 * This XDP program runs on the HMS DSA conduit interface. For every
 * data-path frame it:
 *
 *   1. Parses the HMS DSA tag (a tag_8021q VLAN tag whose VID encodes
 *      the source switch port / switch id and whose PCP carries the
 *      priority).
 *   2. Writes those fields into the XDP metadata area in front of the
 *      packet using bpf_xdp_adjust_meta(). The metadata travels with
 *      the frame into the AF_XDP UMEM but is NOT part of the on-wire
 *      packet.
 *   3. Strips the 4-byte VLAN tag with bpf_xdp_adjust_head() so the
 *      frame seen by userspace starts at a clean Ethernet header.
 *   4. Redirects the frame to an AF_XDP socket bound to the conduit's
 *      RX queue. If no socket is bound, falls back to XDP_PASS.
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
 * Build:
 *   clang -O2 -g -target bpf -c xdp_dsa_meta.c -o xdp_dsa_meta.o
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
int xdp_dsa_meta(struct xdp_md *ctx)
{
	void *data = (void *)(long)ctx->data;
	void *data_end = (void *)(long)ctx->data_end;
	struct ethhdr *eth = data;
	struct xdp_dsa_meta *meta;
	__u32 queue_id = ctx->rx_queue_index;
	__be16 *tci_p;
	__u16 ethproto, tci, vid;
	__u8 src_port, switch_id, pcp;

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
	 * carried in eth->h_proto). Read it and decode the VID/PCP.
	 */
	tci_p = (void *)(eth + 1);
	if ((void *)(tci_p + 1) > data_end)
		return XDP_PASS;

	tci = bpf_ntohs(*tci_p);
	vid = tci & HMS_TCI_VID_MASK;
	pcp = (tci >> HMS_TCI_PCP_SHIFT) & HMS_TCI_PCP_MASK;

	/*
	 * Only act on genuine DSA tag_8021q VIDs (RSV bits == 3). A real
	 * customer VLAN that happens to use 0x8100 is left untouched for
	 * the kernel stack.
	 */
	if (((vid >> HMS_VID_RSV_SHIFT) & HMS_VID_RSV_MASK) != HMS_VID_RSV_VAL)
		return XDP_PASS;

	src_port = (vid >> HMS_VID_PORT_SHIFT) & HMS_VID_PORT_MASK;
	switch_id = (vid >> HMS_VID_SWITCH_SHIFT) & HMS_VID_SWITCH_MASK;

	/*
	 * Reserve metadata room in front of the packet. The verifier
	 * requires the adjust call and the subsequent bounds check to be
	 * done before touching the metadata area.
	 */
	if (bpf_xdp_adjust_meta(ctx, -(int)sizeof(struct xdp_dsa_meta)) < 0)
		return XDP_PASS;

	/* Re-read pointers; data_meta now points to our metadata region. */
	data = (void *)(long)ctx->data;
	data_end = (void *)(long)ctx->data_end;
	meta = (void *)(long)ctx->data_meta;

	if ((void *)(meta + 1) > data)
		return XDP_PASS;

	/* Re-validate the Ethernet + VLAN tag after the meta adjust. */
	eth = data;
	if ((void *)(eth + 1) > data_end)
		return XDP_PASS;

	/* Fill in the metadata descriptor. */
	meta->src_port = src_port;
	meta->switch_id = switch_id;
	meta->ipv = pcp;
	meta->flags = XDP_DSA_META_F_VALID;

	/*
	 * Strip the 4-byte VLAN tag by moving the packet start forward
	 * past the tag. This rewrites the Ethernet header in place: the
	 * original DMAC/SMAC are preserved and the inner ethertype that
	 * followed the tag becomes the new h_proto.
	 *
	 * Layout before strip:
	 *   [ DMAC | SMAC | TPID | TCI | inner_type | payload ]
	 *                  \---- 4-byte VLAN tag ----/
	 * Layout after strip (head advanced by HMS_VLAN_TAG_LEN):
	 *   [ DMAC | SMAC | inner_type | payload ]
	 *
	 * We relocate DMAC+SMAC over the tag bytes first, then push the
	 * head past the now-duplicated leading bytes.
	 */
	{
		__u8 *p = data;
		int i;

		/* Move the 12-byte L2 addresses forward over the tag. */
		#pragma clang loop unroll(full)
		for (i = (2 * ETH_ALEN) - 1; i >= 0; i--) {
			__u8 *dst = p + HMS_VLAN_TAG_LEN + i;
			__u8 *src = p + i;

			if ((void *)(dst + 1) > data_end ||
			    (void *)(src + 1) > data_end)
				return XDP_PASS;
			*dst = *src;
		}
	}

	if (bpf_xdp_adjust_head(ctx, HMS_VLAN_TAG_LEN) < 0)
		return XDP_PASS;

	meta = (void *)(long)ctx->data_meta;
	data = (void *)(long)ctx->data;
	if ((void *)(meta + 1) <= data)
		meta->flags |= XDP_DSA_META_F_TAG_STRIPPED;

	/*
	 * Redirect to the AF_XDP socket bound to this RX queue. If no
	 * socket is present, deliver to the normal stack (without the
	 * DSA tag the stack will no longer demux to hms0pX, so this path
	 * is mainly a safety fallback during bring-up).
	 */
	return bpf_redirect_map(&xsk_map, queue_id, XDP_PASS);
}

char _license[] SEC("license") = "GPL";
