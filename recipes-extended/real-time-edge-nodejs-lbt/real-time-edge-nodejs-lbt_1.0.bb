DESCRIPTION = "Latency and Bandwidth Tester is a web application for generating and \
plotting iPerf and ping traffic."

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=f17a9bec9db0496c87aec8b93aa0d3c6"

# to fix npm-shrinkwrap.json
# SRC_URI = "npmsw://${THISDIR}/files/npm-shrinkwrap.json;dev=1”

SRC_URI += " \
    file://client.js \
    file://config.json \
    file://flows.json \
    file://index.html \
    file://LICENSE \
    file://package.json \
    file://server.js \
    file://S95lbt \
"

DEPENDS += "feedgnuplot real-time-edge-prl"

S = "${WORKDIR}/${BP}"
UNPACKDIR = "${S}"

inherit npm

do_install:append() {
    install -d ${D}/${sysconfdir}/init.d/
    install ${UNPACKDIR}/S95lbt ${D}/${sysconfdir}/init.d/
}

FILES:${PN} += "${libdir}/node_modules/lbt/*"
