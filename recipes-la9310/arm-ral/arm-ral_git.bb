SUMMARY = "ARM RAM is a library for implementing ARM NEON Optimized functions for soft radio processing"
DESCRIPTION = "ARM RAM is a library for implementing ARM NEON Optimized functions for soft radio processing."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=c6ea33711aac321ea52816d2a4d63600"

SRC_URI = "git://gitlab.arm.com/networking/ral.git;protocol=https;nobranch=1 \
"

SRCREV = "9b4ecc36ac4984e1b1077e266c1756281543da7b"

S = "${WORKDIR}/git"

#DEPENDS = "sysrepo"

inherit cmake pkgconfig

PACKAGE_ARCH = "${MACHINE_ARCH}"

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DBUILD_TESTING=On -DBUILD_EXAMPLES=On -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "

FILES:${PN}= "/usr/bin/*"

do_install () {
	install -d ${D}/usr/bin/
	install -m 0755 ${S}/../build/ldpc_decoding ${D}/usr/bin/
	install -m 0755 ${S}/../build/ldpc_encoding ${D}/usr/bin/
}
