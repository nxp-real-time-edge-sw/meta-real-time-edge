
KERNEL_SRC_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-linux.git;protocol=ssh;nobranch=1"
SRCBRANCH_real-time-edge = "linux_5.10.y"
SRCREV_real-time-edge = "d1435e3ef8afbe9427ad4a332f9ff55d9defc4fb"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_real-time-edge = " \
    file://linux-rt.config \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-selinux.config \
    file://linux-fec.config \
"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
