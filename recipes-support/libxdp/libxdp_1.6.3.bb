SUMMARY = "Library for XDP handling"
DESCRIPTION = "Lbrary for working with the eXpress Data Path facility of the Linux kernel"
HOMEPAGE = "https://github.com/xdp-project/xdp-tools"
LICENSE = "LGPL-2.1-or-later & GPL-2.0-or-later & BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=9ee53f8d06bbdb4c11b1557ecc4f8cd5 \
                    file://LICENSES/LGPL-2.1;md5=b370887980db5dd40659b50909238dbd \
                    file://LICENSES/GPL-2.0;md5=994331978b428511800bfbd17eea3001 \
                    file://LICENSES/BSD-2-Clause;md5=5d6306d1b08f8df623178dfd81880927"

DEPENDS += " libbpf zlib elfutils libpcap"
DEPENDS += " clang-cross-${TARGET_ARCH} bpftool-native"

# Use the libbpf provided as a separate system recipe (sysroot) rather than the
# libbpf git submodule bundled inside xdp-tools. This keeps a single libbpf in
# the image and avoids duplicate/conflicting libbpf symbol versions. The
# xdp-tools SRC_URI below intentionally does not fetch submodules, and
# FORCE_SYSTEM_LIBBPF makes configure fail fast if the system libbpf cannot be
# found instead of silently falling back to the (absent) submodule.
SRC_URI = "git://github.com/xdp-project/xdp-tools.git;nobranch=1;protocol=https \
             file://0001-configure-properly-quote-the-toolchain-tools-variabl.patch \
             file://0002-defines.mk-Add-missing-prefix-map-settings-for-OE-bu.patch \
             file://0003-Makefile-It-does-not-detect-libbpf-header-from-sysro.patch \
             file://0004-Makefile-fix-KeyError-failure.patch \
             file://0005-Makefile-fix-libxdp.pc-error.patch \
             file://0006-xdp-trafficgen-support-XDP-ETF.patch \
             file://0007-xdp-trafficgen-add-config-and-script-for-XDP-ETF.patch \
           "

# xdp-tools v1.6.3
SRCREV = "8fbad9f0af621a22aa87ff2520b3735915b1f0fd"

inherit pkgconfig

EXTRA_OEMAKE = "PREFIX=${prefix} V=1 DESTDIR=${D} \
                BPF_DIR_MNT='/sys/fs/bpf' OE_BPF_OBJECT_DIR='/usr/lib/bpf' \
                CLANG='${TARGET_PREFIX}clang --sysroot=${RECIPE_SYSROOT}' \
                BPFTOOL=bpftool \
"

export PRODUCTION = "1"
export FORCE_SYSTEM_LIBBPF = "1"
export BPFTOOL = "bpftool"

export STAGING_INCDIR

do_configure:append () {
    export PRODUCTION=1
    export FORCE_SYSTEM_LIBBPF=1
    export BPFTOOL=bpftool
    ${S}/configure
}

do_compile:prepend:aarch64 () {
    mkdir -p ${S}/headers/asm
    cp ${RECIPE_SYSROOT}/usr/include/asm/bitsperlong-64.h ${S}/headers/asm/bitsperlong-32.h
    cp ${RECIPE_SYSROOT}/usr/include/asm/byteorder-64.h ${S}/headers/asm/byteorder-32.h
    cp ${RECIPE_SYSROOT}/usr/include/asm/posix_types-64.h ${S}/headers/asm/posix_types-32.h
    cp ${RECIPE_SYSROOT}/usr/include/asm/swab-64.h ${S}/headers/asm/swab-32.h
    cp ${RECIPE_SYSROOT}/usr/include/asm/types-64.h ${S}/headers/asm/types-32.h
}

do_install () {
    oe_runmake install
    install -m 0755 ${S}/xdp-trafficgen/xdp-etf-run.sh ${D}/usr/sbin/
    install -m 0644 ${S}/xdp-trafficgen/etf_config.txt ${D}/usr/sbin/
}
