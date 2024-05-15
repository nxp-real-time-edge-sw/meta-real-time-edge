
include ../../../../recipes-kernel/include/la93xx-repo.inc

UBOOT_SRC = "${REAL_TIME_EDGE_UBOOT_SRC}"
SRCBRANCH = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV = "${REAL_TIME_EDGE_UBOOT_SRCREV}"
PATCHTOOL = "git"

# For BareMetal
REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2022.04-2.5.0"
REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV ?= "9c383729197af50fbb041fc107335c73870b4db8"
SRCBRANCH:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH}"
SRCREV:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV}"

SRC_URI = "${UBOOT_SRC};branch=${SRCBRANCH}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append:imx8mp-rfnm = " \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
