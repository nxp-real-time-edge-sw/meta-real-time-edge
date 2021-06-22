
UBOOT_SRC_rt-edge = "git://bitbucket.sw.nxp.com/dnind/industry-uboot.git;protocol=ssh"
UBOOT_BRANCH_rt-edge = "uboot_v2020.04"
SRCREV_rt-edge = "01fea3c25ecf4b572dc6e337d628734058630176"

SRC_URI_rt-edge = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure_prepend_rt-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
