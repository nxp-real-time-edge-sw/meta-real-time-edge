DESCRIPTION = "Latency and Bandwidth Tester is a web application for generating and \
plotting iPerf and ping traffic."

LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${WORKDIR}/LICENSE;md5=5ab514dd2789d25115ba18df005856e2"

SRC_URI = "file://client.js \
           file://config.json \
	   file://flows.json \
	   file://index.html \
	   file://LICENSE \
	   file://package.json \
	   file://server.js \
	   file://S95lbt \
"

DEPENDS = "nodejs nodejs-native feedgnuplot rt-edge-prl"

S = "${WORKDIR}"

do_install (){
    install -d ${D}/${libdir}/node_modules/lbt
    install -d ${D}/${sysconfdir}/init.d/
    install ${S}/server.js ${D}/${libdir}/node_modules/lbt/
    install ${S}/client.js ${D}/${libdir}/node_modules/lbt/
    install ${S}/index.html ${D}/${libdir}/node_modules/lbt/
    install ${S}/config.json ${D}/${libdir}/node_modules/lbt/
    install ${S}/package.json ${D}/${libdir}/node_modules/lbt/
    install ${S}/S95lbt ${D}/${sysconfdir}/init.d/
    cd ${D}/${libdir}/node_modules/lbt
    npm install --only=production
    chown -R root:root ${D}/${libdir}/node_modules/lbt
}

FILES_${PN} += "${libdir}/node_modules/lbt/*"
