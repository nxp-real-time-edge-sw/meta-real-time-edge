
SRC_URI_real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-linux.git;protocol=https;nobranch=1"
KERNEL_BRANCH_real-time-edge = "linux_5.10.72"
SRCREV_real-time-edge = "d1435e3ef8afbe9427ad4a332f9ff55d9defc4fb"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_real-time-edge = " \
    file://linux-rt.config \
    file://linux-wifi.config \
    file://linux-baremetal-ls104xa.config \
    file://linux-baremetal-ls1021a.config \
    file://linux-baremetal-ls1028a.config \
    file://linux-baremetal-lx2160a.config \
    file://linux-selinux.config \
    file://linux-dpaa-ethercat.config \
"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
