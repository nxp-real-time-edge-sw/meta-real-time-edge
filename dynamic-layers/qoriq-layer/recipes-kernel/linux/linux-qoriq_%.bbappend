
SRC_URI_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-linux.git;protocol=ssh;nobranch=1"
KERNEL_BRANCH_real-time-edge = "linux_5.10.y"
SRCREV_real-time-edge = "d448c556ceaefb46eabde032be090f95ae386389"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_real-time-edge = " \
    file://linux-rt.config \
    file://linux-wifi.config \
    file://linux-baremetal-ls104xa.config \
    file://linux-baremetal-ls1021a.config \
    file://linux-baremetal-ls1028a.config \
    file://linux-baremetal-lx2160a.config \
    file://linux-selinux.config \
"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
