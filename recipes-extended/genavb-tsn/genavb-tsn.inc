LICENSE = "NXP-Binary-EULA & GPLv2 & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/EULA.txt;md5=358cdbb1e99e9302cf3abc0152a1ade4 \
                    file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_URL = "git://bitbucket.sw.nxp.com/mss/gen_avb_sdk.git;protocol=ssh"

GENAVB_TSN_BRANCH = "dev/5_0"
SRC_URI = "${GENAVB_TSN_URL};branch=${GENAVB_TSN_BRANCH}"

SRCREV = "${AUTOREV}"

PV = "5.0+git${SRCPV}"

S = "${WORKDIR}/git"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG:imx8mp-lpddr4-evk = "endpoint_tsn"
GENAVB_TSN_CONFIG:ls1028ardb = "bridge"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET:imx8mp-lpddr4-evk = "linux_imx8"
GENAVB_TSN_TARGET:ls1028ardb = "linux_ls1028"

