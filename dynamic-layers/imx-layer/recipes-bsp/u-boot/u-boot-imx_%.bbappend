
REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2026.04-3.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "2862cb153d69fd51e56942aa65c2ca52444edd3c"

UBOOT_SRC:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRC}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRCREV}"

# For BareMetal
REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2026.04-3.5.0"
REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV ?= "d2ed2d9c1501fd09ae61eb8e9865ed97018b2252"
SRCBRANCH:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH}"
SRCREV:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV}"

SRC_URI:real-time-edge = "${UBOOT_SRC};branch=${SRCBRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
