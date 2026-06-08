# XDP DSA Per-Port RX Examples - User Guide

## 1. Overview

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
single conduit interface (for example `eth0`).

For the design background, frame formats, kernel data path and the
hardware queue mapping analysis, see the companion document
[`xdp-dsa-design.md`](./xdp-dsa-design.md). This guide focuses on the
practical build-and-run workflow.

## 2. What is in the package

The recipe `xdp-dsa-examples` builds and installs the following artifacts,
all co-located under `/examples/xdp-dsa-examples`:

| Artifact | Installed location | Purpose |
|----------|--------------------|---------|
| `xdp_dsa_rx_monitor`  | `/examples/xdp-dsa-examples` | AF_XDP monitor; parses the DSA tag in userspace. |
| `xdp_dsa_port_rx`     | `/examples/xdp-dsa-examples` | AF_XDP receiver; reads the source port from XDP metadata. |
| `xdp_dsa_redirect.o`  | `/examples/xdp-dsa-examples` | BPF program that only redirects frames to AF_XDP (no tag strip). |
| `xdp_dsa_meta.o`      | `/examples/xdp-dsa-examples` | BPF program that records the port in XDP metadata and strips the tag. |


There are two complementary userspace applications:

- `xdp_dsa_rx_monitor` loads `xdp_dsa_redirect.o`. The BPF program only
  redirects frames to the AF_XDP socket; the DSA tag stays on the frame
  and the application parses the tag in userspace to find the source
  port. This is the simplest variant and good for inspection.

- `xdp_dsa_port_rx` loads `xdp_dsa_meta.o`. The BPF program parses the
  DSA tag, writes the source port (and switch id, priority, flags) into
  the XDP metadata area in front of the packet, strips the 4-byte DSA
  tag, and then redirects the frame. The application reads the source
  port straight from the metadata, so it never has to see or parse the
  DSA tag. This variant also demonstrates the zero-copy data path.

Both applications bind to the conduit interface (for example `eth0`),
not to the `hms0pX` user ports.

## 3. Prerequisites

- A Real-time Edge image built for a supported i.MX machine with the HMS
  DSA switch driver enabled, and the switch user ports (`hms0pX`)
  present.
- The image must contain `libxdp` and `libbpf` (these are pulled in as
  build dependencies of the recipe; for runtime they ship as shared
  libraries used by the applications).
- Root privileges on the target (loading XDP programs and creating
  AF_XDP sockets require `CAP_NET_ADMIN` / `CAP_BPF`).

## 4. Building

### 4.1 Add the recipe to your image

Add the package to your image, for example by appending to your local
`conf/local.conf`:

```
IMAGE_INSTALL:append = " xdp-dsa-examples"
```

Then build your image as usual, for example:

```bash
DISTRO=nxp-real-time-edge MACHINE=imx8mp-lpddr4-evk \
    source real-time-edge-setup-env.sh -b build-imx8mpevk-real-time-edge
bitbake nxp-image-real-time-edge
```

### 4.2 Build only the example package

To build just the examples (for example while iterating):

```bash
bitbake xdp-dsa-examples
```

The recipe compiles the two BPF object files with the native `clang`
(`clang -target bpf`) and cross-compiles the two userspace applications
against the target `libxdp` / `libbpf`.

## 5. Running on the target

Both applications must run as root and bind to the conduit interface.
Each application loads its companion BPF object file (`xdp_dsa_redirect.o`
or `xdp_dsa_meta.o`) from its install directory `/examples/xdp-dsa-examples`,
which is baked into the binary at build time. You can therefore launch the
applications from any working directory; there is no need to `cd` into the
install directory or copy the `.o` files around.

If you relocate the `.o` files (for example when running locally built
binaries from a build tree), point the applications at the new directory
with the `XDP_DSA_OBJDIR` environment variable:

```bash
export XDP_DSA_OBJDIR=/path/to/dir/with/objects
```

The install directory is not on `PATH` by default, so the shell will not
find the binaries by their bare name. Use one of these equivalent forms:

- Full path (used throughout this guide), from any directory:

  ```bash
  /examples/xdp-dsa-examples/xdp_dsa_port_rx -i eth0 -q 0 -v
  ```

- `cd` into the directory and run with a leading `./` (the `./` is
  required; a bare `xdp_dsa_port_rx` is still resolved through `PATH`,
  not the current directory):

  ```bash
  cd /examples/xdp-dsa-examples/
  ./xdp_dsa_port_rx -i eth0 -q 0 -v
  ```

- Add the directory to `PATH` once, then the bare name works:

  ```bash
  export PATH="/examples/xdp-dsa-examples:$PATH"
  xdp_dsa_port_rx -i eth0 -q 0 -v
  ```

In all three forms the `.o` files are loaded correctly regardless of the
current working directory, because the install path is baked into the
binaries; `cd` only affects how the shell resolves the binary name.



### 5.1 Bring up the interfaces

Make sure the conduit interface and the relevant switch user ports are
up:

```bash
ip link set eth0 up
ip link set hms0p0 up
ip link set hms0p1 up
```

> The conduit interface name on your platform may differ from `eth0`.
> Check `ls /sys/class/net` and the switch topology in your device tree.
> The switch user ports are named `hms0pX`.

### 5.2 xdp_dsa_rx_monitor (userspace tag parsing)

```bash
/examples/xdp-dsa-examples/xdp_dsa_rx_monitor -i eth0 -q 0 -v
```

Options:

| Option | Description |
|--------|-------------|
| `-i <ifname>` | Conduit interface name (required), for example `eth0`. |
| `-q <queue>`  | RX queue id to bind the AF_XDP socket to (default `0`). |
| `-v`          | Verbose output (hex dump of the first bytes of each frame). |
| `-h`          | Show help. |

The monitor prints one line per received frame with the decoded source
port (shown as `hms0pX`), switch id, source/destination MAC, inner
ethertype and length, and a per-port packet/byte summary on exit
(Ctrl+C).

### 5.3 xdp_dsa_port_rx (metadata + zero-copy)

```bash
/examples/xdp-dsa-examples/xdp_dsa_port_rx -i eth0 -q 0 -v
```

Options:

| Option | Description |
|--------|-------------|
| `-i <ifname>` | Conduit interface name (required), for example `eth0`. |
| `-q <queue>`  | RX queue id to bind the AF_XDP socket to (default `0`). |
| `-z`          | Require zero-copy (`XDP_ZEROCOPY`); fail if the driver/queue does not support it. |
| `-c`          | Force copy mode (`XDP_COPY`). |
| `-S`          | Use SKB / generic XDP mode (default is native driver mode). |
| `-v`          | Verbose output (hex dump). |
| `-h`          | Show help. |

Notes on the data path:

- With neither `-z` nor `-c`, the kernel auto-negotiates and prefers
  zero-copy when the driver supports it, silently falling back to copy
  mode otherwise.
- `-z` and `-c` are mutually exclusive.
- Zero-copy (`-z`) requires native mode, so it cannot be combined with
  `-S`.

Because the BPF program (`xdp_dsa_meta.o`) strips the DSA tag, the frames
delivered to this application no longer carry the VLAN tag. The source
port is read from the XDP metadata (`struct xdp_dsa_meta`) placed in
front of each packet.

## 6. How frames are classified

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

## 7. Troubleshooting

| Symptom | Likely cause / action |
|---------|-----------------------|
| `Failed to attach XDP program` | Not running as root, or the driver does not support native XDP on this interface. Try `-S` (generic mode) with `xdp_dsa_port_rx`. |
| `Failed to create XSK socket: ... zero-copy` hint | The driver/queue does not support zero-copy. Drop `-z` (auto) or use `-c` (copy). |
| `Interface <name> not found` | Wrong interface name; confirm with `ls /sys/class/net`. Bind to the conduit interface, not `hms0pX`. |
| No frames seen | Confirm the conduit and `hms0pX` ports are `up`, that traffic is actually arriving on a switch port, and that you bound to the correct RX queue (`-q`). |
| Cannot open `xdp_dsa_redirect.o` / `xdp_dsa_meta.o` | The application loads the `.o` from `/examples/xdp-dsa-examples` (baked in at build time). Confirm the files exist there, or point the app at their actual location with `export XDP_DSA_OBJDIR=/path/to/dir`. |

## 8. References

- Kernel sources: `net/dsa/tag_hms.c`, `net/dsa/tag_8021q.c`,
  `include/linux/dsa/8021q.h`.
- libxdp / libbpf documentation for the AF_XDP / XSK APIs used by the
  applications.
