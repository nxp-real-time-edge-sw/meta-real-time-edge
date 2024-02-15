# DPDK version update with bbdev support
# Build tools and examples here

require ../../recipes-kernel/include/la93xx-repo.inc

LICENSE = "BSD-3-Clause & LGPL-2.1-only & GPL-2.0-only"
LIC_FILES_CHKSUM = "file://license/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://license/lgpl-2.1.txt;md5=4b54a1fd55a448865a0b32d41598759d \
                    file://license/bsd-3-clause.txt;md5=0f00d99239d922ffd13cabef83b33444"

DPDK_SRC = "git://git@bitbucket.sw.nxp.com/gitam/dpdk.git;protocol=ssh"
#SRCREV = "5479bb82b12306a32283d2ef88efb63a696e0d78"
#DPDK_SRC = "git://github.com/nxp/dpdk;protocol=https"
SRCREV = "0decf58dfea29e7feb3cbd45c8dc69c044138f20"

SRC_URI = "${DPDK_SRC};nobranch=1 \
        ${SRC_LA9310_FRTOS_URI};destsuffix=la93xx_freertos \
"

#DEPENDS:append = "freertos-la9310"

S = "${WORKDIR}/git"

DPDK_EXAMPLES:imx-nxp-bsp = "l2fwd,l3fwd,bbdev_app,bbdev_raw_app,pkt_gen_ext,pkt_split_app "

do_configure:prepend() {
	export LA9310_COMMON_HEADERS="${WORKDIR}/la93xx_freertos/common_headers"
}

do_compile:prepend() {
	export LA9310_COMMON_HEADERS="${WORKDIR}/la93xx_freertos/common_headers"
}

do_install:append(){
    cp ${S}/nxp/debug_dump.sh ${D}/
}