
REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2025.04-3.4.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "fb3db400b8036143a202b70b62ad0984b934579a"

UBOOT_SRC:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRC}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRCREV}"

# For BareMetal
REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2025.04-3.4.0"
REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV ?= "76d0c501d140fa0ca6623f4eb18adbc6fe0cf025"
SRCBRANCH:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_BRANCH}"
SRCREV:baremetal = "${REAL_TIME_EDGE_UBOOT_BAREMETAL_SRCREV}"

SRC_URI:real-time-edge = "${UBOOT_SRC};branch=${SRCBRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
