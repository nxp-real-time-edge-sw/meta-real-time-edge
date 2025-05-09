DESCRIPTION = "This is one demo to use OpenCV in NXP platforms."
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

SRC_URI = "file://app.py \
	   file://ip.py \
"

S = "${UNPACKDIR}"

do_install() {
	install -d -m 0755 ${D}/usr/demo
	install -p ${UNPACKDIR}/*.py ${D}/usr/demo
}

FILES:${PN} += " \
	/usr/demo/app.py \
	/usr/demo/ip.py \
"
