# XDP DSA Per-Port RX Examples - User Guide

## Overview

This guide explains how to build, deploy and run the DSA XDP / AF_XDP
example applications on a Real-time Edge i.MX target. The examples show
how to receive frames coming from the ports of an HMS DSA switch on the
host (conduit) interface, identify which switch port a frame came from,
and process the frames in userspace through an AF_XDP socket.

The HMS switch uses the kernel `tag_8021q` DSA tagging scheme: the switch
inserts a standard 4-byte 802.1Q (or 802.1AD) VLAN tag after the source
MAC address, and the source switch port and switch id are encoded in the
VLAN VID. The kernel exposes one network interface per switch user port,
named `hms0pX` (for example `hms0p0`, `hms0p1`, ...), all riding on a
single conduit interface (for example `eth2` on i.MX 943).

For more details, refer to "Heterogeneous multi-SoC framework" in Real-time Edge User Guide.

These files are installed in the directory `/examples/xdp-dsa-examples` of the file system:

| Artifact | Installed location | Purpose |
|----------|--------------------|---------|
| `xdp_dsa_rx_monitor`  | `/examples/xdp-dsa-examples` | AF_XDP monitor; parses the DSA tag in userspace. |
| `xdp_dsa_port_rx`     | `/examples/xdp-dsa-examples` | AF_XDP receiver; reads the source port from XDP metadata. |
| `xdp_dsa_redirect.o`  | `/examples/xdp-dsa-examples` | BPF program that only redirects frames to AF_XDP (no tag strip). |
| `xdp_dsa_meta.o`      | `/examples/xdp-dsa-examples` | BPF program that records the port in XDP metadata and strips the tag. |

There are two complementary userspace applications:

- `xdp_dsa_rx_monitor` loads `xdp_dsa_redirect.o`. The BPF program only
  redirects frames to the AF_XDP socket. The DSA tag stays on the frame
  and the application parses the tag in userspace to find the source
  port. This is the simplest variant and good for inspection.

- `xdp_dsa_port_rx` loads `xdp_dsa_meta.o`. The BPF program parses the
  DSA tag, writes the source port (and switch id, priority, flags) into
  the XDP metadata area in front of the packet, strips the 4-byte DSA
  tag, and then redirects the frame. The application reads the source
  port straight from the metadata, so it never has to see or parse the
  DSA tag. This variant also demonstrates the zero-copy data path.

## Hardware setup

Use i.MX 943 EVK + i.MX RT1180 EVK as an example.

### Preparing the i.MX RT1180 EVK

The i.MX RT1180 EVK runs a FreeRTOS-based DSA switch application that
uses one of its external switch ports (`ENET3`) as the DSA CPU port. Before wiring
the boards together, flash the `dsa_switch.elf` image to the RT1180:

1. Copy `dsa_switch.elf` to the host PC. Pre-built images are included in
   the root filesystem at `/examples/heterogeneous-multi-soc/dsa-switch-evkmimxrt1180-cm33/release/`.

2. Connect the host PC to the MCU-Link USB connector **J53** on the
   RT1180 EVK using a micro-USB cable.

3. Set **SW5[1..4]** to **0000** (SDP mode), then use the MCUXpresso
   Secure Provisioning Tool (SPT) to burn `dsa_switch.elf` to the
   on-board flash.

4. After flashing, set **SW5** to **0100** (flash-boot mode) and power
   cycle the board.

For the detailed step-by-step flashing procedure, see
*Hardware preparation for i.MX RT1180 EVK* in the Real-time Edge User
Guide (HMS hardware setup chapter).

### SPI connection (DSA control plane)

Use flying leads to connect i.MX 943 EVK LPSPI3 pins on **J47** to
i.MX RT1180 EVK LPSPI3 pins on **J44**:

| i.MX 943 EVK Pin | Function     | Connection   | i.MX RT1180 EVK Pin | Function     |
|-------------------|--------------|--------------|---------------------|--------------|
| 6                 | LPSPI3_PCS0  | connected to | 6                   | LPSPI3_PCS0  |
| 8                 | LPSPI3_MOSI  | connected to | 10                  | LPSPI3_SIN   |
| 10                | LPSPI3_MISO  | connected to | 8                   | LPSPI3_SOUT  |
| 12                | LPSPI3_CLK   | connected to | 12                  | LPSPI3_CLK   |
| 14                | GND          | connected to | 14                  | GND          |

### Ethernet connections

Two Ethernet cables are needed:

1. **DSA control-plane link** — connect:
   - i.MX 943 EVK ENETC2 (`eth2` in Linux) on **J27** RJ45
   - i.MX RT1180 EVK NETC switch port 3 (ENET3) on **J31** RJ45

2. **Traffic path** — connect:
   - i.MX 943 EVK ENETC1 (`eth1` in Linux) on **J26** RJ45
   - i.MX RT1180 EVK switch port 0 (ENET0) on **J28** RJ45

Traffic injected on `eth1` enters the RT1180 switch at port 0 and is
forwarded to the DSA CPU port, arriving on the conduit
interface `eth2` with a DSA tag. This is the traffic the XDP examples
capture.

## Running on the target

### Select the HMS DSA device tree on the i.MX 943 EVK

Before booting Linux on the i.MX 943 EVK, select the HMS DSA device tree
blob `imx943-evk-hms-dsa.dtb` so that the kernel enables the HMS switch driver and
creates the DSA conduit and user-port interfaces.

After Linux boots, you should see the conduit interface `eth2` and
the per-port DSA user interfaces (`hms0p0`, `hms0p1`, ...).

### Bring up the interfaces

Make sure the DSA conduit interface and the relevant switch user ports are
up. In this test, `eth2` on i.MX 943 is used as the DSA conduit interface for the HMS switch.

```bash
ip link set eth2 up
ip link set hms0p0 up
ip link set hms0p1 up
```

### Pre-test configuration

Before running the XDP examples, apply these one-time setup steps on the target.

* **Disable VLAN offloads.** The DSA tag looks like a VLAN tag to the ENETC.
  The hardware VLAN offload would strip it before XDP sees the frame.
  Therefore, the offload feature must be disabled:

```bash
ethtool -K eth2 rx-vlan-offload off
ethtool -K eth2 tx-vlan-offload off
```

* **Disable receive hashing on eth2.** Make sure all network traffic is received on Queue 0.

```bash
ethtool -K eth2 receive-hashing off
```

* **Skip the libxdp dispatcher.** The examples use libbpf directly and do
  not go through the libxdp multi-program dispatcher. Tell libxdp to skip
  its dispatcher check:

```bash
export LIBXDP_SKIP_DISPATCHER=1
```

* **Generate test traffic on eth1.** Start `pktgen` to inject traffic into switch port 0
  via the `eth1` link (for example, one packet every 2 seconds):

```bash
cd /usr/share/samples/pktgen; while true; do ./pktgen_sample01_simple.sh -i eth1 -m be:a0:97:f3:c7:1f -n 1 > /dev/null 2>&1; sleep 2; done &
```
Note: replace the MAC address with the actual MAC address of `eth2` on your target.

### Run xdp_dsa_rx_monitor (userspace tag parsing)

```bash
/examples/xdp-dsa-examples/xdp_dsa_rx_monitor -i eth2 -v
```

The application prints information per received frame with the decoded source
port (shown as `hms0pX`), switch id, source/destination MAC, inner
ethertype and length.

**Example output:**

```
DSA XDP RX Monitor
==================
Interface: eth2 (index 4)
Queue: 0
Press Ctrl+C to stop

XDP program loaded and attached to eth2
XSK socket created on eth2 queue 0

Monitoring packets...

[22:38:21.309317] hms0p1 (switch 0): 0a:34:30:27:46:0f -> be:a0:97:f3:c7:1f proto=0x0800 len=64
  Hex: be a0 97 f3 c7 1f 0a 34 30 27 46 0f 81 00 0c 01
       08 00 45 00 00 2e 00 00 00 00 20 11 b3 ce 0a c1
       15 f4 c6 12 00 2a 00 3c 00 09 00 1a 00 00 be 9b
       e9 55 00 00 00 01 00 00 00 00 00 00 00 00 00 00
[22:38:23.937397] hms0p1 (switch 0): 0a:34:30:27:46:0f -> be:a0:97:f3:c7:1f proto=0x0800 len=64
  Hex: be a0 97 f3 c7 1f 0a 34 30 27 46 0f 81 00 0c 01
       08 00 45 00 00 2e 00 00 00 00 20 11 b3 ce 0a c1
       15 f4 c6 12 00 2a 00 45 00 09 00 1a 00 00 be 9b
       e9 55 00 00 00 01 00 00 00 00 00 00 00 00 00 00
```

### Run xdp_dsa_port_rx (metadata + zero-copy)

```bash
/examples/xdp-dsa-examples/xdp_dsa_port_rx -i eth2 -v
```

Because the BPF program (`xdp_dsa_meta.o`) strips the DSA tag, the frames
delivered to this application no longer carry the VLAN tag. The source
port is read from the XDP metadata (`struct xdp_dsa_meta`) placed in
front of each packet.

**Example output:**

```
DSA XDP Port RX (metadata-based)
================================
Interface: eth2 (index 4)
Queue: 0
XDP mode: native
Data path: auto (zero-copy preferred)
Press Ctrl+C to stop

XDP program loaded and attached to eth2
XSK socket created on eth2 queue 0

Monitoring packets...

[21:49:27.817274] hms0p0 (switch 0, ipv 0): 0a:34:30:27:46:0f -> be:a0:97:f3:c7:1f proto=0x0800 len=60 [tag-stripped]
  Hex: be a0 97 f3 c7 1f 0a 34 30 27 46 0f 08 00 45 00
       00 2e 00 00 00 00 20 11 f9 03 a9 fe 31 81 c6 12
       00 2a 00 47 00 09 00 1a 00 00 be 9b e9 55 00 00
       00 01 00 00 00 00 00 00 00 00 00 00
[21:49:30.425773] hms0p0 (switch 0, ipv 0): 0a:34:30:27:46:0f -> be:a0:97:f3:c7:1f proto=0x0800 len=60 [tag-stripped]
  Hex: be a0 97 f3 c7 1f 0a 34 30 27 46 0f 08 00 45 00
       00 2e 00 00 00 00 20 11 f9 03 a9 fe 31 81 c6 12
       00 2a 00 61 00 09 00 1a 00 00 be 9b e9 55 00 00
       00 01 00 00 00 00 00 00 00 00 00 00
```

## How frames are classified

The BPF programs and the monitor only redirect genuine DSA `tag_8021q`
data frames to AF_XDP. A frame is treated as a DSA-tagged data frame only
when:

- the outer ethertype is `0x8100` (C-tag) or `0x88A8` (S-tag), and
- the top two VID bits (`VID[11:10]`) equal `3`, which marks a DSA
  `tag_8021q` VID.

The source port is `VID[3:0]` and the switch id is `VID[8:6]`.

Frames that must be handled by the kernel DSA stack are intentionally
NOT redirected and continue up the normal stack:

- PTP frames (`01:1B:19:xx:xx:xx`) and slow-protocol link-local frames
  (`01:80:C2:xx:xx:xx`);
- HMS in-band meta / timestamp frames (ethertype `0xDADC`).

This keeps PTP, link-local protocols and switch control traffic working
normally while only the data-plane frames are diverted to AF_XDP.

## Troubleshooting

| Symptom | Likely cause / action |
|---------|-----------------------|
| `Failed to attach XDP program` | Not running as root, or the driver does not support native XDP on this interface. Try `-S` (generic mode) with `xdp_dsa_port_rx`. |
| `Failed to create XSK socket: ... zero-copy` hint | The driver/queue does not support zero-copy. Drop `-z` (auto) or use `-c` (copy). |
| `Interface <name> not found` | Wrong interface name; confirm with `ls /sys/class/net`. Bind to the conduit interface, not `hms0pX`. |
| No frames seen | Confirm the conduit and `hms0pX` ports are `up`, that traffic is actually arriving on a switch port, and that you bound to the correct RX queue (`-q`). |
| Cannot open `xdp_dsa_redirect.o` / `xdp_dsa_meta.o` | The application loads the `.o` from `/examples/xdp-dsa-examples` (baked in at build time). Confirm the files exist there, or point the app at their actual location with `export XDP_DSA_OBJDIR=/path/to/dir`. |

## References

- Kernel sources: `net/dsa/tag_hms.c`, `net/dsa/tag_8021q.c`, `include/linux/dsa/8021q.h`.
- libxdp/libbpf documentation for the AF_XDP / XSK APIs used by the applications.
