LICENSE = "NXP-Binary-EULA & GPLv2 & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/EULA.txt;md5=448263cd692e8b91d7bce7ca865fe362 \
                    file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_URL = "git://github.com/NXP/GenAVB_TSN.git;protocol=https"

SRC_URI = "${GENAVB_TSN_URL};nobranch=1 "

SRCREV = "d9eec1e83c6747422956d0cc6289caf22a004e05"

PV = "5.0+git${SRCPV}"

S = "${WORKDIR}/git"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG:imx8mp-lpddr4-evk = "endpoint_tsn"
GENAVB_TSN_CONFIG:ls1028ardb = "bridge"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET:imx8mp-lpddr4-evk = "linux_imx8"
GENAVB_TSN_TARGET:ls1028ardb = "linux_ls1028"

