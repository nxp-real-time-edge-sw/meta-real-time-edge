SUMMARY = "pktgen-scripts"
DESCRIPTION = "pktgen-scripts contains scripts used to generate traffic using pktgen"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://LICENSE \
           file://pktgen"

S = "${WORKDIR}"

do_install() {
    install -d ${D}/home/root/samples/pktgen
    install -Dm 0755 ${WORKDIR}/pktgen/* ${D}/home/root/samples/pktgen/
}

FILES:${PN} += "/home/root/samples/pktgen/*"

RDEPENDS:${PN} = "bash"
