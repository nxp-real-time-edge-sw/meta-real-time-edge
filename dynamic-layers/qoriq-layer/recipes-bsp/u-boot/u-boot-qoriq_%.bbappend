
SRC_URI:real-time-edge = "git://bitbucket.sw.nxp.com/dnind/real-time-edge-uboot.git;protocol=ssh;nobranch=1"
UBOOT_BRANCH:real-time-edge = "uboot_v2021.04"
SRCREV:real-time-edge = "20ef92b8ca6010b9974bd151b7691ffadb7d1e63"

do_configure:prepend:real-time-edge() {
    mkdir -p ${WORKDIR}/source-date-epoch
    date '+%s' > ${WORKDIR}/source-date-epoch/__source_date_epoch.txt
}
