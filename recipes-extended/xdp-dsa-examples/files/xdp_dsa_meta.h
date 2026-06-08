/* SPDX-License-Identifier: GPL-2.0 */
/*
 * DSA XDP shared definitions (HMS switch)
 *
 * Common HMS DSA tag layout and the XDP metadata descriptor that is
 * shared between the BPF program (xdp_dsa_meta.c) and the AF_XDP
 * userspace application (xdp_dsa_port_rx.c).
 *
 * The BPF program parses the HMS DSA tag on the conduit interface,
 * records the source switch port (and a few related fields) into the
 * XDP metadata area in front of the packet, strips the DSA tag, and
 * redirects the frame to an AF_XDP socket. The userspace application
 * then reads the metadata to learn which switch port the frame came
 * from, without having to re-parse (or even see) the DSA tag.
 *
 * HMS uses the generic DSA tag_8021q scheme: the switch encodes the
 * source port and switch id in the VID of a VLAN tag (TPID 0x8100 C-tag
 * or 0x88A8 S-tag). This matches the kernel tagger net/dsa/tag_hms.c
 * and the VID bit layout in net/dsa/tag_8021q.c.
 *
 * Reference kernel sources (real-time-edge-linux):
 *   net/dsa/tag_hms.c
 *   net/dsa/tag_8021q.c
 *   include/linux/dsa/hms.h
 *   include/linux/dsa/8021q.h
 *   drivers/net/dsa/hms/
 *   arch/arm64/boot/dts/freescale/imxrt1180-hms-switch.dtsi
 *
 * Copyright 2026 NXP
 */

#ifndef XDP_DSA_META_H
#define XDP_DSA_META_H

/*
 * HMS switch DSA tag (tag_8021q based).
 *
 * On the data path the switch presents each frame to the conduit MAC
 * with a VLAN tag inserted right after the source MAC address. The
 * VLAN VID carries the source switch port / switch id, and the VLAN
 * PCP carries the priority. The tag is followed by the real inner
 * ethertype and payload.
 *
 *   +--------+--------+-----------------+-----------+-----------------+
 *   | DMAC   | SMAC   | VLAN tag        | inner type| payload         |
 *   | 6B     | 6B     | 4B (TPID+TCI)   | 2B        | ...             |
 *   +--------+--------+-----------------+-----------+-----------------+
 *                     ^
 *                     | starts at offset 2 * ETH_ALEN (12)
 *
 * The VLAN tag is the standard 4-byte 802.1Q/802.1AD tag:
 *
 *   +-------------------+-------------------------------------------+
 *   | TPID (2B)         | TCI (2B): PCP[15:13] DEI[12] VID[11:0]    |
 *   +-------------------+-------------------------------------------+
 *
 * Two TPIDs are possible depending on bridge VLAN protocol:
 *   - HMS_VLAN_TPID_C  (0x8100) : C-tag (ETH_P_8021Q)
 *   - HMS_VLAN_TPID_S  (0x88A8) : S-tag (ETH_P_HMS / 802.1AD)
 *
 * In addition to the data path, HMS uses an in-band control extension
 * with ethertype HMS_META_ETHERTYPE (0xDADC, ETH_P_HMS_META) to carry
 * meta-frames and hardware timestamps. Those frames, as well as PTP and
 * slow-protocol link-local frames, are intended for the kernel DSA
 * stack and must NOT be redirected to AF_XDP.
 */

/* VLAN ethertypes used by the HMS tag_8021q data path. */
#define HMS_VLAN_TPID_C		0x8100	/* ETH_P_8021Q  C-tag */
#define HMS_VLAN_TPID_S		0x88A8	/* ETH_P_HMS    S-tag (802.1AD) */

/* In-band control extension ethertype (meta / timestamp frames). */
#define HMS_META_ETHERTYPE	0xDADC	/* ETH_P_HMS_META */

/* 802.1Q VLAN tag is 4 bytes (TPID + TCI). */
#define HMS_VLAN_TAG_LEN	4

/*
 * tag_8021q VID bit layout (see net/dsa/tag_8021q.c):
 *
 *   | 11 | 10 |  9  |  8 |  7 |  6 |  5 |  4 |  3 |  2 |  1 |  0 |
 *   +----+----+-----+----+----+----+----+----+----+----+----+----+
 *   |   RSV   | VBID|  SWITCH_ID   |  VBID   |        PORT        |
 *   +---------+-----+--------------+---------+-------------------+
 *
 *   RSV       VID[11:10] : must equal 3 (0b11) to be a DSA 8021q VID
 *   SWITCH_ID VID[8:6]   : DSA tree switch index (0..7)
 *   PORT      VID[3:0]   : switch port index (0..15)
 *
 * PORT is the source switch port we want, SWITCH_ID is the switch id.
 */
#define HMS_VID_RSV_SHIFT	10
#define HMS_VID_RSV_MASK	0x03
#define HMS_VID_RSV_VAL		3

#define HMS_VID_SWITCH_SHIFT	6
#define HMS_VID_SWITCH_MASK	0x07

#define HMS_VID_PORT_SHIFT	0
#define HMS_VID_PORT_MASK	0x0F

/* VLAN TCI field accessors. */
#define HMS_TCI_VID_MASK	0x0FFF
#define HMS_TCI_PCP_SHIFT	13
#define HMS_TCI_PCP_MASK	0x07

/*
 * XDP metadata descriptor placed in front of the packet via
 * bpf_xdp_adjust_meta(). This does NOT become part of the on-wire
 * frame: it lives in the headroom in front of ctx->data and is
 * delivered to the AF_XDP application in the UMEM frame, just before
 * the packet data pointed to by the RX descriptor.
 *
 * Keep this structure small and naturally aligned. The first field is
 * src_port so a consumer that only cares about the port can read a
 * single leading byte.
 */
struct xdp_dsa_meta {
	__u8 src_port;		/* switch port the frame came from (VID PORT) */
	__u8 switch_id;		/* switch ID (VID SWITCH_ID) */
	__u8 ipv;		/* priority (VLAN PCP) */
	__u8 flags;		/* see XDP_DSA_META_F_* below */
};

/* flags bits */
#define XDP_DSA_META_F_TAG_STRIPPED	(1U << 0) /* DSA tag removed from frame */
#define XDP_DSA_META_F_VALID		(1U << 1) /* metadata fields are valid */

#endif /* XDP_DSA_META_H */
