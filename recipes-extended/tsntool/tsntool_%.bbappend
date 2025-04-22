DESCRIPTION = "A tool to configure TSN funtionalities in user space for industrial"

SAVED_DIR := "${THISDIR}"

SRC_URI = "git://github.com/nxp-qoriq/tsntool;protocol=https;nobranch=1"

SRCREV:qoriq = "dbe0feb2bcf7bf2be2c009cf82398f9fee30ff89"
SRCREV:imx-nxp-bsp = "dbe0feb2bcf7bf2be2c009cf82398f9fee30ff89"

PREFIX ?= "/usr"
BINDIR ?= "${PREFIX}/bin"
INCLUDEDIR ?= "${PREFIX}/include"
LIBDIR ?= "${PREFIX}/lib"
TSN_LIB_PC = "libtsn.pc"

do_compile:append() {
    make ${TSN_LIB_PC}
}

do_install:append() {
    install -d -m 0755 ${D}${BINDIR}
    install -d -m 0755 ${D}${LIBDIR}
    install -d -m 0755 ${D}${INCLUDEDIR}/tsn
    install -d -m 0755 ${D}${INCLUDEDIR}/linux/
    install -m 0644 include/tsn/genl_tsn.h ${D}${INCLUDEDIR}/tsn
    install -D -m 644 include/linux/tsn.h ${D}${INCLUDEDIR}/linux/
    install -D -m 644 ${TSN_LIB_PC} ${D}${LIBDIR}/pkgconfig/libtsn.pc
    install -d ${D}${datadir}/samples
    install -d ${D}${datadir}/samples/gatescripts
    install -d ${D}${datadir}/samples/cncdemo
    install -Dm 0755 ${SAVED_DIR}/samples/gatescripts/* ${D}${datadir}/samples/gatescripts/
    install -Dm 0755 ${S}/demos/cnc/topoagent.py ${D}${datadir}/samples/cncdemo/
}

FILES:${PN} += " \
    ${INCLUDEDIR}/tsn \
    ${INCLUDEDIR}/linux \
    ${LIBDIR}/pkgconfig/libtsn.pc \
    ${datadir}/samples \
    ${datadir}/samples/gatescripts \
    ${datadir}/samples/cncdemo \
"
COMPATIBLE_MACHINE = "qoriq|imx"

INSANE_SKIP:${PN} += "dev-deps"
