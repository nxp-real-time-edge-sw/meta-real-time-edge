FILESEXTRAPATHS:prepend := "${THISDIR}/${BPN}:"

BUSYBOX_SPLIT_SUID:real-time-edge-plc = "0"
ALTERNATIVE_PRIORITY[init] = "40"

SRC_URI:append:real-time-edge-plc = " file://defconfig-plc"

do_configure:prepend:real-time-edge-plc () {
    cp ${WORKDIR}/defconfig-plc ${WORKDIR}/defconfig
}


