
UBOOT_SRC_rt-edge = "git://bitbucket.sw.nxp.com/dnind/industry-uboot.git;protocol=ssh"
UBOOT_BRANCH_rt-edge = "uboot_v2020.04"
SRCREV_rt-edge = "16cc6eaf985f284090ad28b6403e0b1e1f8a9828"

do_configure_prepend_rt-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
