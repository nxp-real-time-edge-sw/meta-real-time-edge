SUMMARY = "YANG data modelling language parser and toolkit"
DESCRIPTION = "libyang is YANG data modelling language parser and toolkit written (and providing API) in C. The library is used e.g. in libnetconf2, Netopeer2 or sysrepo projects."
SECTION = "libs"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b69fd3b2815bbf1cef5c97f0eee2519a"

SRC_URI = "git://github.com/CESNET/libyang.git;protocol=https;branch=master \
        file://0001-validation-workaround-for-a-must-validation-issue.patch"

PV = "3.13.5+git"
SRCREV = "efe43e3790822a3dc64d7d28db935d03fff8b81f"

S = "${WORKDIR}/git"

DEPENDS = "libpcre2"

FILES:${PN} += "${datadir}/yang/modules/libyang/*"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE:String=Release "

BBCLASSEXTEND = "native nativesdk"
