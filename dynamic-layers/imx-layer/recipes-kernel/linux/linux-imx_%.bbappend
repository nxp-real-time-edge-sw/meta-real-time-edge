
KERNEL_SRC_rt-edge = "git://bitbucket.sw.nxp.com/dnind/industry-linux.git;protocol=ssh"
SRCBRANCH_rt-edge = "linux_5.10.y"
SRCREV_rt-edge = "0830a043168b40f5bc552ff11a78f845ac3de1c8"

FILESEXTRAPATHS_prepend := "${THISDIR}/${PN}:"
SRC_URI_append_rt-edge = " \
    file://linux-rt.config \
    file://linux-imx8.config \
    file://linux-wifi.config \
    file://linux-baremetal.config \
    file://linux-selinux.config \
    file://linux-fec.config \
"

do_configure_prepend_rt-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
