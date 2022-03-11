
UBOOT_SRC:real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh;nobranch=1"
UBOOT_BRANCH:real-time-edge = "uboot_v2021.04"
SRCREV:real-time-edge = "4951c6e0515f040db45ebbbd816c635614accff0"

SRC_URI:real-time-edge = "${UBOOT_SRC};branch=${UBOOT_BRANCH}"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
