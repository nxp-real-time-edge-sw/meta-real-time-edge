LICENSE = "GPL-2.0-or-later & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_SRC ?= "git://github.com/NXP/GenAVB_TSN.git;protocol=https"
GENAVB_TSN_SRCREV ?= "609be86df6fd472c8b32e9eef201ffa2a0b46f78"

SRC_URI = "${GENAVB_TSN_SRC};"

SRCREV = "${GENAVB_TSN_SRCREV}"

PV = "7.1+git${SRCPV}"

S = "${WORKDIR}/git"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG:mx8mp-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:mx93-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:mx8dxl-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:ls1028a = "bridge"
GENAVB_TSN_CONFIG:mx95-nxp-bsp = "endpoint_tsn"
GENAVB_TSN_CONFIG:mx943-nxp-bsp = "endpoint_tsn"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET:mx8mp-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:mx93-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:mx8dxl-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:ls1028a = "linux_ls1028"
GENAVB_TSN_TARGET:mx95-nxp-bsp = "linux_imx8"
GENAVB_TSN_TARGET:mx943-nxp-bsp = "linux_imx8"
