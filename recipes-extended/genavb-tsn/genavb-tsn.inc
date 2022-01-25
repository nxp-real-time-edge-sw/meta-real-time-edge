LICENSE = "NXP-Binary-EULA & GPLv2 & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/EULA.txt;md5=d67589208881475633e9a262e3905274 \
                    file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_URL = "git://bitbucket.sw.nxp.com/mss/gen_avb_sdk.git;protocol=ssh"

SRC_URI = "${GENAVB_TSN_URL};nobranch=1 "

SRCREV = "9d936ee86786db6139a69a126ad19f9daf4841e1"

PV = "4.4+git${SRCPV}"

S = "${WORKDIR}/git"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG_imx8mpevk = "endpoint_tsn"
GENAVB_TSN_CONFIG_ls1028ardb = "bridge"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET_imx8mpevk = "linux_imx8"
GENAVB_TSN_TARGET_ls1028ardb = "linux_ls1028"

