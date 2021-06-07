DESCRIPTION = "A tool to configure TSN funtionalities in user space for industrial"

SAVED_DIR := "${THISDIR}"

#SRC_URI_industrial = "git://github.com/openil/tsntool.git;protocol=https"
SRC_URI = "git://bitbucket.sw.nxp.com/dnind/tsntool.git;protocol=ssh;nobranch=1"
#SRCREV_industrial = "e0bd2f6fd9b066bc8da09aeddd88c6237e0872a4"

SRCREV_qoriq = "b30a3fbad987832e7ed45fdebaa91a9f7f21128e"
SRCREV_imx = "b30a3fbad987832e7ed45fdebaa91a9f7f21128e"

FILES_${PN} += "/root/* /root/sample/* /root/samples/* \
               /root/samples/gatescripts/* /root/samples/pktgen/* \
               /root/samples/cncdemo/* \
	       /usr/* /usr/include/* /usr/include/linux/* /usr/include/tsn/* \
	       /usr/lib/* /usr/lib/pkgconfig/* \
	       "
PREFIX ?= "/usr"
BINDIR ?= "${PREFIX}/bin"
INCLUDEDIR ?= "${PREFIX}/include"
LIBDIR ?= "${PREFIX}/lib"
TSN_LIB_PC = "libtsn.pc"

do_compile_append() {
    make ${TSN_LIB_PC}
}

do_install_append() {
    install -d -m 0755 ${D}${BINDIR}
    install -d -m 0755 ${D}${LIBDIR}
    install -d -m 0755 ${D}${INCLUDEDIR}/tsn
    install -d -m 0755 ${D}/${INCLUDEDIR}/linux/
    install -m 0644 include/tsn/genl_tsn.h ${D}/${INCLUDEDIR}/tsn
    install -D -m 644 include/linux/tsn.h ${D}/${INCLUDEDIR}/linux/
    install -D -m 644 ${TSN_LIB_PC} ${D}/${LIBDIR}/pkgconfig/libtsn.pc
    install -d ${D}/root/samples
    install -d ${D}/root/samples/gatescripts
    install -d ${D}/root/samples/pktgen
    install -d ${D}/root/samples/cncdemo
    install -Dm 0755 ${SAVED_DIR}/samples/gatescripts/* ${D}/root/samples/gatescripts/
    install -Dm 0755 ${SAVED_DIR}/samples/pktgen/* ${D}/root/samples/pktgen/
    install -Dm 0755 ${S}/demos/cnc/topoagent.py ${D}/root/samples/cncdemo/
}

COMPATIBLE_MACHINE = "qoriq|imx"
