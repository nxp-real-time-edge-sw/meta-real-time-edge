
SRC_URI:real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-uboot.git;protocol=https;nobranch=1"
UBOOT_BRANCH:real-time-edge = "uboot_v2021.04"
SRCREV:real-time-edge = "ebabb5117875949c38f3265e06c19e57a027b134"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
