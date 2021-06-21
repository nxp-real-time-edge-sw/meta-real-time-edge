
UBOOT_SRC_rt-edge = "git://bitbucket.sw.nxp.com/dnind/industry-uboot.git;protocol=ssh"
UBOOT_BRANCH_rt-edge = "uboot_v2020.04"
SRCREV_rt-edge = "b7e7593ce19f78938d24c41308543a2be977bfbc"

SRC_URI_rt-edge = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure_prepend_rt-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
