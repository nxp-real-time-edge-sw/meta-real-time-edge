
KERNEL_SRC_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-linux.git;protocol=ssh"
SRCBRANCH_real-time-edge = "linux_5.10.y"
SRCREV_real-time-edge = "ba8eba9b25ba683f46db9ab0b7b9508dad9fe9a4"

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
