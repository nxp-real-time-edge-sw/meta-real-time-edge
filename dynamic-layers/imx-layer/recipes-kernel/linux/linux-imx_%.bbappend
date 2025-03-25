
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_6.12.3-imx943-er1"
REAL_TIME_EDGE_LINUX_SRCREV ?= "96499c0249fc256f863ab2847aa28be76321c4e9"

KERNEL_SRC:mx943-nxp-bsp = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCBRANCH:mx943-nxp-bsp = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:mx943-nxp-bsp = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI:mx943-nxp-bsp = "${KERNEL_SRC}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append:real-time-edge = " \
    file://linux-baremetal.config \
    file://linux-baremetal-imx93.config \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
