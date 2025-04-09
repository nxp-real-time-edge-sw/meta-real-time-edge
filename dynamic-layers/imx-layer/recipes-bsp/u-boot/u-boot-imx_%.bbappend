
REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2025.04-3.2.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "57a8448c0b4a9545c0768f46acca30d77fbe25f8"

UBOOT_SRC = "${REAL_TIME_EDGE_UBOOT_SRC}"
SRCBRANCH = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV = "${REAL_TIME_EDGE_UBOOT_SRCREV}"

# For BareMetal
REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2025.04-3.2.0"
REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV ?= "8fcf48c7c728f23ee12115ced8749e5a0b3b1c57"
SRCBRANCH:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH}"
SRCREV:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV}"

SRC_URI = "${UBOOT_SRC};branch=${SRCBRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
