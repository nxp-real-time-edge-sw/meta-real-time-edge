
SRC_URI:real-time-edge = "git://github.com/real-time-edge-sw/real-time-edge-uboot.git;protocol=https;nobranch=1"
UBOOT_BRANCH:real-time-edge = "baremetal-uboot_v2021.04-gpio"
SRCREV:real-time-edge = "2923fdeab3b07ec0bc40023bc3543a40abdc3eda"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
