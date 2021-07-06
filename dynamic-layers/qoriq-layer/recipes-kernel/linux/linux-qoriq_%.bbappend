
KERNEL_SRC_rt-edge = "git://github.com/rt-edge-sw/rt-edge-linux.git;protocol=https"
SRCBRANCH_rt-edge = "linux_5.10.y"
SRCREV_rt-edge = "baf692faaebd0fc5f085274124128ffdc1b09403"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_rt-edge = " \
    file://linux-rt.config \
    file://linux-wifi.config \
    file://linux-baremetal-ls104xa.config \
    file://linux-baremetal-ls1021a.config \
    file://linux-baremetal-ls1028a.config \
    file://linux-baremetal-lx2160a.config \
    file://linux-selinux.config \
"

do_configure_prepend_rt-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
