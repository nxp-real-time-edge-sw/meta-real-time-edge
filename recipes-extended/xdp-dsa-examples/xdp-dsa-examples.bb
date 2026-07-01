SUMMARY = "DSA XDP / AF_XDP per-port RX examples for the HMS switch"
DESCRIPTION = "Sample XDP and AF_XDP applications that receive DSA tag_8021q \
tagged frames on the conduit interface, identify the source HMS switch port, \
optionally strip the DSA tag and report the source port via XDP metadata. \
Includes two userspace applications (xdp_dsa_rx_monitor, xdp_dsa_port_rx) and \
two BPF programs (xdp_dsa_redirect.o, xdp_dsa_meta.o)."
HOMEPAGE = "https://github.com/nxp-real-time-edge-sw/real-time-edge"
SECTION = "examples"

LICENSE = "GPL-2.0-or-later"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0-or-later;md5=fed54355545ffd980b814dab4a3b312c"

# libbpf and libxdp provide the AF_XDP/XSK and XDP loading APIs used by the
# userspace applications, plus the headers needed to build them. clang-native
# is used to compile the BPF object files (clang -target bpf), which must be
# built natively rather than cross-compiled. elfutils and zlib satisfy the
# link-time dependencies pulled in by libxdp/libbpf.
DEPENDS = "libbpf libxdp clang-native elfutils zlib"

SRC_URI = " \
    file://Makefile \
    file://xdp_dsa_meta.c \
    file://xdp_dsa_meta.h \
    file://xdp_dsa_port_rx.c \
    file://xdp_dsa_redirect.c \
    file://xdp_dsa_rx_monitor.c \
    file://xdp-dsa-user-guide.md \
"

S = "${UNPACKDIR}"

# Directory where the applications and the BPF object files are installed.
# Both the userspace binaries and the .o files live here, under /examples, so
# the example set is self-contained in one directory. The applications load
# the .o files from this directory by default (the path is baked in at build
# time via -DXDP_DSA_OBJDIR), so they can be launched from any working
# directory.
XDP_DSA_INSTALLDIR = "/examples/xdp-dsa-examples"

do_compile() {
    # The BPF objects are compiled by clang with -target bpf. clang does not
    # pick up the target sysroot include path automatically for this target,
    # so the libbpf headers (bpf/bpf_helpers.h) are passed in explicitly via
    # BPF_CFLAGS (-I${STAGING_INCDIR}). The userspace apps use the normal cross
    # CFLAGS/LDFLAGS.
    #
    # The -target bpf triple does not predefine the host architecture macros
    # (__aarch64__ / __LP64__). Without them the cross sysroot glibc header
    # bits/wordsize.h takes its 32-bit branch and the UAPI asm/types.h pulls in
    # asm/types-32.h, which is not staged for aarch64 (only asm/types-64.h is).
    # Define __TARGET_ARCH_arm64 (the libbpf convention) and force the 64-bit
    # word size so the UAPI headers resolve to the aarch64 variants.
    #
    # XDP_DSA_OBJDIR is the runtime install directory baked into the userspace
    # apps so they can locate the .o files regardless of the current working
    # directory.
    oe_runmake CC="${CC}" CLANG="clang" \
        CFLAGS="${CFLAGS}" LDFLAGS="${LDFLAGS}" \
        XDP_DSA_OBJDIR="${XDP_DSA_INSTALLDIR}" \
        BPF_CFLAGS="-O2 -g -target bpf -D__TARGET_ARCH_arm64 -D__aarch64__ -D__LP64__ -I${STAGING_INCDIR}"
}

do_install() {
    # Install the applications, BPF object files and the user guide together
    # under /examples, so the example set is self-contained in one directory.
    install -d ${D}${XDP_DSA_INSTALLDIR}
    install -m 0755 ${B}/xdp_dsa_rx_monitor ${D}${XDP_DSA_INSTALLDIR}/
    install -m 0755 ${B}/xdp_dsa_port_rx ${D}${XDP_DSA_INSTALLDIR}/
    install -m 0644 ${B}/xdp_dsa_redirect.o ${D}${XDP_DSA_INSTALLDIR}/
    install -m 0644 ${B}/xdp_dsa_meta.o ${D}${XDP_DSA_INSTALLDIR}/
    install -m 0644 ${UNPACKDIR}/xdp-dsa-user-guide.md ${D}${XDP_DSA_INSTALLDIR}/
}

FILES:${PN} = "${XDP_DSA_INSTALLDIR}"


# The BPF object files embed host build paths in their BTF/debug sections; this
# is expected for clang -target bpf objects and is not a real QA problem.
INSANE_SKIP:${PN} = "buildpaths"

# AF_XDP / DSA tag_8021q support depends on the HMS switch driver, which is
# only available on the i.MX platforms used by Real-time Edge.
COMPATIBLE_MACHINE = "imx"
