SUMMARY = "rfnm cli application for RF control"
SECTION = "rfnm cli"
LICENSE = "CLOSED"

SRC_URI = "\
            file://rfnm-cli.tar.gz \
        "

S = "${WORKDIR}/rfnm-cli/cli"

inherit cmake

EXTRA_OECMAKE = ""

FILES:${PN}= "/usr/bin/*"

do_install () {
        install -d ${D}/usr/bin
        install -m 0755 ${S}/build/cli ${D}/usr/bin
}

