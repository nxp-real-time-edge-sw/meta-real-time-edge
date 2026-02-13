
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_6.18.2"
REAL_TIME_EDGE_LINUX_SRCREV ?= "b351e88c3cdcfed25f727cb21eca483366ca8502"

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI:real-time-edge = "${KERNEL_SRC}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:baremetal:mx8m-nxp-bsp = " file://linux-baremetal.config"
SRC_URI:append:baremetal:mx93-nxp-bsp = " file://linux-baremetal-imx93.config"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
