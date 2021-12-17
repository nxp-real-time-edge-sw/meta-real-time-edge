
SRC_URI_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh;nobranch=1"
UBOOT_BRANCH_real-time-edge = "uboot_v2021.04"
SRCREV_real-time-edge = "bed31ab581267e329d9d0b9a92d8fb8fea67d874"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
