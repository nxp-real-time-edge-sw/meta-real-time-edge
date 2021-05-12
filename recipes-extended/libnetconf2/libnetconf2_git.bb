SUMMARY = "libnetconf2 is a NETCONF library in C intended for building NETCONF clients and servers"
DESCRIPTION = "The library provides functions to connect NETCONF client and server to each other via SSH."
SECTION = "libs"
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b1f886606973edff74d729043d78afef"

SRC_URI = "git://github.com/CESNET/libnetconf2.git;protocol=https;nobranch=1\
	   file://0001-libnetconf2-fix-the-issue-of-sprintf.patch \
"

#PV = "0.12.55+git${SRCPV}"
SRCREV = "7879bf38ca67f77cf492ede0a40ba6364cf07815"

S = "${WORKDIR}/git"

DEPENDS = "doxygen cmocka libssh openssl libyang libxcrypt"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE:String=Release -DENABLE_SSH=ON -DENABLE_TLS=ON -DENABLE_BUILD_TESTS=OFF -DENABLE_VALGRIND_TESTS=OFF "

BBCLASSEXTEND = "native nativesdk"
