
SRC_URI:real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh;nobranch=1"
UBOOT_BRANCH:real-time-edge = "uboot_v2021.04"
SRCREV:real-time-edge = "5e08bb0ff902cbd9859371c003d79623df20f778"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
