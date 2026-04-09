
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_6.18.2"
REAL_TIME_EDGE_LINUX_SRCREV ?= "8d8d7edbd8aa20969e4febd8ec0200873733a4f4"

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCBRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI:real-time-edge = "${KERNEL_SRC}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI:append:baremetal:mx8m-nxp-bsp = " file://linux-baremetal.cfg"
SRC_URI:append:baremetal:mx93-nxp-bsp = " file://linux-baremetal-imx93.cfg"

SRC_URI:append = "${@bb.utils.contains('DISTRO_FEATURES', 'rpmsg_8m_buf', ' file://linux-rpmsg-8m-buf.cfg', '', d)}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
