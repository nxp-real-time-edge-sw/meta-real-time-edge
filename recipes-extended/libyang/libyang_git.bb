SUMMARY = "YANG data modelling language parser and toolkit"
DESCRIPTION = "libyang is YANG data modelling language parser and toolkit."
SECTION = "libs"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=2982272c97a8e417a844857ca0d303b1"

SRC_URI = "git://github.com/CESNET/libyang.git;protocol=https;nobranch=1"

#PV = "v1.0-r4+git${SRCPV}"
SRCREV = "64248ec90179740e66416c78e0b3b914a8a07d30"

S = "${WORKDIR}/git"

DEPENDS = "libpcre"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE:String=Release "

BBCLASSEXTEND = "native nativesdk"
