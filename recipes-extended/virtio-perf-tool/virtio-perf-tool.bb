SUMMARY = "virtio performance test tool"
DESCRIPTION = "virtio-perf-tool contains scripts to test the performance of Heterogeneous-Multicore VirtIO"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=e6a75371ba4d16749254a51215d13f97"

SRC_URI = "file://LICENSE \
           file://vt_test.sh"

S = "${WORKDIR}"

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${WORKDIR}/vt_test.sh ${D}${bindir}
}

FILES:${PN} += "${bindir}/vt_test.sh"

RDEPENDS:${PN} = "bash"
