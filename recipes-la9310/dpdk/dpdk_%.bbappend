# DPDK version update with bbdev support
# Build tools and examples here

require ${BSPDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo.inc


LICENSE = "BSD-3-Clause & LGPL-2.1-only & GPL-2.0-only"
LIC_FILES_CHKSUM = "file://license/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://license/lgpl-2.1.txt;md5=4b54a1fd55a448865a0b32d41598759d \
                    file://license/bsd-3-clause.txt;md5=0f00d99239d922ffd13cabef83b33444"

SRCREV_FORMAT = "la93xx-sdk"

SRC_URI = "${DPDK_SRC}; \
        ${SRC_LA9310_FRTOS_URI};destsuffix=la93xx_freertos \
"
SRCREV="${SRCREV_dpdk_rev}"

#DEPENDS:append = "freertos-la9310"

S = "${WORKDIR}/git"

DPDK_EXAMPLES:imx-nxp-bsp = "l2fwd,l3fwd,bbdev_app,bbdev_raw_app,pkt_gen_ext,pkt_split_app,dfe_app "

do_configure:prepend() {
	export LA9310_COMMON_HEADERS="${WORKDIR}/la93xx_freertos/common_headers"
}

do_compile:prepend() {
	export LA9310_COMMON_HEADERS="${WORKDIR}/la93xx_freertos/common_headers"
}

FILES:${PN} = "/usr/bin/* ${ROOT_HOME}/*"

do_install:append(){
    # Install examples
    install -d ${D}/${ROOT_HOME}
    install -m 0755 ${S}/nxp/debug_dump.sh ${D}/${ROOT_HOME}/

    install -m 0755 -d ${D}/usr/bin/
    install -m 0755 ${S}/nxp/debug_dump.sh ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-bbdev_raw_app  ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-l2fwd  ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-l3fwd  ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-pkt_gen_ext  ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-pkt_split_app ${D}/usr/bin/
    install -m 0755 ${B}/examples/dpdk-dfe_app ${D}/usr/bin/
}