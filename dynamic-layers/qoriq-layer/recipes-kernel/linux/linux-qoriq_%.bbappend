
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-linux.git;protocol=https;"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_5.15.52"
REAL_TIME_EDGE_LINUX_SRCREV ?= "0efe9e9618a826b1e19aa134a2d6a58efeb9cce2"

KERNEL_SRC:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC}"
KERNEL_BRANCH:real-time-edge = "${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"
SRC_URI = "${KERNEL_SRC};branch=${KERNEL_BRANCH}"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
SRC_URI:append:real-time-edge = " \
    file://linux-wifi.config \
    file://linux-baremetal-ls104xa.config \
    file://linux-baremetal-ls1021a.config \
    file://linux-baremetal-ls1028a.config \
    file://linux-baremetal-lx2160a.config \
    file://linux-selinux.config \
    file://linux-dpaa-ethercat.config \
"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
