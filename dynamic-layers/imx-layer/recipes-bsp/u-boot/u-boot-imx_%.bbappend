
UBOOT_SRC:real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh;nobranch=1"
UBOOT_BRANCH:real-time-edge = "uboot_v2021.04"
SRCREV:real-time-edge = "d6c66f35189540b0fa72291914e72d6dcb45372c"

SRC_URI:real-time-edge = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
