SUMMARY = "isochron"
DESCRIPTION = "The isochron program is a real-time application for testing Time Sensitive Networking equipment."
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=8ca43cbc842c2336e835926c2166c28b"

DEPENDS = "libmnl"

inherit pkgconfig

SRC_URI = "git://github.com/NXP/isochron.git;protocol=https;branch=master"
SRCREV = "7f06efd5b40ef1cfb6210883bc4307bac338adc0"

# Fix GCC 15 warning: assignment discards 'const' qualifier
TARGET_CFLAGS:append = " -Wno-error=discarded-qualifiers"

do_compile() {
    oe_runmake isochron
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/isochron ${D}${bindir}
}

