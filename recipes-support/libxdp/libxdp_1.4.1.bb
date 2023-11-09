SUMMARY = "Library for XDP handling"
DESCRIPTION = "Lbrary for working with the eXpress Data Path facility of the Linux kernel"
HOMEPAGE = "https://github.com/xdp-project/xdp-tools"
LICENSE = "LGPL-2.1-or-later & GPL-2.0-or-later & BSD-2-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=9ee53f8d06bbdb4c11b1557ecc4f8cd5 \
                    file://LICENSES/LGPL-2.1;md5=b370887980db5dd40659b50909238dbd \
                    file://LICENSES/GPL-2.0;md5=994331978b428511800bfbd17eea3001 \
                    file://LICENSES/BSD-2-Clause;md5=5d6306d1b08f8df623178dfd81880927"

DEPENDS += " libbpf clang-native"

SRC_URI = "git://github.com/xdp-project/xdp-tools.git;branch=master;protocol=https \
           file://0001-configure-properly-quote-the-toolchain-tools-variabl.patch \
           file://0002-libxdp-add-option-to-skip-building-xdp-bpf-objects-w.patch \
           file://0003-libxdp-do-not-preserve-ownership-when-copying-librar.patch \
           "

SRCREV = "2236e8d286701f275881da77bf1eef43d53d370f"

S = "${WORKDIR}/git"

inherit pkgconfig

EXTRA_OEMAKE += "PREFIX=${prefix} DESTDIR=${D} SKIP_XDP_OBJS_BUILD=1"

do_compile () {
    oe_runmake libxdp
}

do_install () {
    oe_runmake libxdp_install
}
