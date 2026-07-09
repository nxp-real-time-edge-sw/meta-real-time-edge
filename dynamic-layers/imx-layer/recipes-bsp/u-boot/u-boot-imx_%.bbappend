
REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2026.04-3.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "c7a7ef138cd44ac292985cae7cfcfc7a799f2ebc"

UBOOT_SRC:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRC}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRCREV}"

# For BareMetal
REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2026.04-3.5.0"
REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV ?= "27f227302a4ccb1372ee650bf7fdcccf37fe3e8f"
SRCBRANCH:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH}"
SRCREV:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV}"

SRC_URI:real-time-edge = "${UBOOT_SRC};branch=${SRCBRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
