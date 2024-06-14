SUMMARY = "ARM RAM is a library for implementing ARM NEON Optimized functions for soft radio processing"
DESCRIPTION = "ARM RAM is a library for implementing ARM NEON Optimized functions for soft radio processing."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://license_terms/BSD-3-Clause.txt;md5=0b78a9675df61b1368982e4641d07fe3"

SRC_URI = "git://gitlab.arm.com/networking/ral.git;protocol=https;nobranch=1 \
"

SRCREV = "2f304504777627e6540137452e52716ed2d82fb9"

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