REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/real-time-edge-sw/real-time-edge-uboot.git;protocol=https;nobranch=1"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2022.04"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "${AUTOREV}"

SRC_URI:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRC};branch=${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRCREV}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
