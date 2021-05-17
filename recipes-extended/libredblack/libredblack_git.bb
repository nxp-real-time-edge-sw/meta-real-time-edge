SUMMARY = "Library for handling red-black tree searching algorithm"
DESCRIPTION = "A library to provide the RedBlack balanced tree searching and sorting algorithm."
LICENSE = "LGPLv2.1"
LIC_FILES_CHKSUM = "file://COPYING;md5=de174fb5a92cdbe038c88dc4c4316f99"

SRC_URI = "git://github.com/sysrepo/libredblack.git;protocol=https"

PV = "1.0+git${SRCPV}"
SRCREV = "a399310d99b61eec4d3c0677573ab5dddcf9395d"

S = "${WORKDIR}/git"

# NOTE: if this software is not capable of being built in a separate build directory
# from the source, you should replace autotools with autotools-brokensep in the
# inherit line
inherit python3native autotools

# Specify any options you want to pass to the configure script using EXTRA_OECONF:
EXTRA_OECONF = " --without-rbgen "

BBCLASSEXTEND = "native nativesdk"
