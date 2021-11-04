DESCRIPTION = "Latency and Bandwidth Tester is a web application for generating and \
plotting iPerf and ping traffic."

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=f17a9bec9db0496c87aec8b93aa0d3c6"

SRC_URI = "npmsw://${THISDIR}/files/npm-shrinkwrap.json;dev=1 \
	   file://client.js;subdir=${BP} \
	   file://config.json;subdir=${BP} \
	   file://flows.json;subdir=${BP} \
	   file://index.html;subdir=${BP} \
	   file://LICENSE \
	   file://package.json;subdir=${BP} \
	   file://server.js;subdir=${BP} \
	   file://S95lbt \
"

DEPENDS += "feedgnuplot real-time-edge-prl"

S = "${WORKDIR}/${BP}"

inherit npm

do_install_append() {
    install -d ${D}/${sysconfdir}/init.d/
    install ${WORKDIR}/S95lbt ${D}/${sysconfdir}/init.d/
}

FILES_${PN} += "${libdir}/node_modules/lbt/*"
