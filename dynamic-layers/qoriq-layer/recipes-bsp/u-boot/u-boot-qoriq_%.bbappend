
UBOOT_SRC_real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-uboot.git;protocol=https;nobranch=1"
UBOOT_BRANCH_real-time-edge = "uboot_v2021.04"
SRCREV_real-time-edge = "ae6f725a51aec225c3cc688e9d9714f6d2e974f2"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
