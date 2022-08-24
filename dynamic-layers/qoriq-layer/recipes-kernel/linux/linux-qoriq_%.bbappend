
REAL_TIME_EDGE_LINUX_SRC ?= "git://github.com/real-time-edge-sw/real-time-edge-linux.git;protocol=https;nobranch=1"
REAL_TIME_EDGE_LINUX_BRANCH ?= "linux_5.15.y"
REAL_TIME_EDGE_LINUX_SRCREV ?= "be88a251b281cbe654728e45b9cb8cab14416355"

SRC_URI:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRC};branch=${REAL_TIME_EDGE_LINUX_BRANCH}"
SRCREV:real-time-edge = "${REAL_TIME_EDGE_LINUX_SRCREV}"

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
