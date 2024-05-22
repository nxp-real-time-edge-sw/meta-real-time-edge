LICENSE = "GPL-2.0-or-later & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_URL = "git://bitbucket.sw.nxp.com/genavbtsn/gen_avb_sdk.git;protocol=ssh"
GENAVB_TSN_BRANCH = "dev/master"

SRC_URI = "${GENAVB_TSN_URL};branch=${GENAVB_TSN_BRANCH}"

SRCREV = "${AUTOREV}"

PV = "master+git${SRCPV}"

S = "${WORKDIR}/git"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG:mx8mp-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:mx93-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:mx8dxl-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:ls1028a = "bridge"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET:mx8mp-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:mx93-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:mx8dxl-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:ls1028a = "linux_ls1028"
