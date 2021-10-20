
UBOOT_SRC_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh"
UBOOT_BRANCH_real-time-edge = "uboot_v2021.04"
SRCREV_real-time-edge = "dc5ccc4f796107d2f31c83b2b7943ca510f0d282"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
