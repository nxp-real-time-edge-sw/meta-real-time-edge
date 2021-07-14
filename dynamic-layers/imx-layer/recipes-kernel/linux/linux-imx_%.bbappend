
KERNEL_SRC_real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-linux.git;protocol=https"
SRCBRANCH_real-time-edge = "linux_5.10.y"
SRCREV_real-time-edge = "baf692faaebd0fc5f085274124128ffdc1b09403"

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
