
REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2022.04-2.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "5fb673f8859674281f3d614c01699efb618ac0c7"

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
    file://0001-rfnm-Add-support-for-rfnm-board.patch \
    file://0001-rfnm-add-workaround-for-hardcoded-physical-address.patch \
    file://0001-rfnm-Add-support-to-read-bootconfig-mem-rage-from-dt.patch \
    file://0001-Changes-enabling-RBB.patch \
    file://0001-update-rfnm-shared.h-file.patch \
    file://0001-rfnm-Move-bootconfig-reserve-memory-from-0xa3400000.patch \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
