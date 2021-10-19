
UBOOT_SRC_real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh"
UBOOT_BRANCH_real-time-edge = "uboot_v2021.04"
SRCREV_real-time-edge = "1a7456dbd25d8515d5cb99816363816e50dee0ea"

SRC_URI_real-time-edge = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure_prepend_real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
