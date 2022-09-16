SUMMARY = "tsn-scripts"
DESCRIPTION = "tsn-scripts is a tool to test TSN features"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=8ca43cbc842c2336e835926c2166c28b"
DEPENDS = "jq"

SRC_URI = "git://github.com/vladimiroltean/tsn-scripts.git;protocol=https;branch=isochron"
SRCREV = "3a1187344e28c1401523d79686035bbd9e1c3e51"

S = "${WORKDIR}/git"

do_compile() {
     oe_runmake -C isochron
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 ${S}/isochron/isochron ${D}${bindir}
}

