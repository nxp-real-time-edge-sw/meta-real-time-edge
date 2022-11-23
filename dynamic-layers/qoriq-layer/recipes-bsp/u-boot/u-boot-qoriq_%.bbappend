REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "uboot_v2022.04"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "a0457cacaf827c3f084c97d7539b4a5b9a0911b5"

UBOOT_SRC:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRC}"
UBOOT_BRANCH:real-time-edge = "${REAL_TIME_EDGE_UBOOT_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_UBOOT_SRCREV}"
SRC_URI = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
