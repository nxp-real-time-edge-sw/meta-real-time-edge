DESCRIPTION = "A tool to configure TSN funtionalities in user space for industrial"

SAVED_DIR := "${THISDIR}"

SRCREV_qoriq = "b3a66928ad10eddc8333bf32912360aa97b7798f"
SRCREV_imx = "b3a66928ad10eddc8333bf32912360aa97b7798f"

FILES_${PN} += "/home/root/* /home/root/sample/* /home/root/samples/* \
               /home/root/samples/gatescripts/* /home/root/samples/pktgen/* \
               /home/root/samples/cncdemo/* \
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
    install -d ${D}/home/root/samples
    install -d ${D}/home/root/samples/gatescripts
    install -d ${D}/home/root/samples/pktgen
    install -d ${D}/home/root/samples/cncdemo
    install -Dm 0755 ${SAVED_DIR}/samples/gatescripts/* ${D}/home/root/samples/gatescripts/
    install -Dm 0755 ${SAVED_DIR}/samples/pktgen/* ${D}/home/root/samples/pktgen/
    install -Dm 0755 ${S}/demos/cnc/topoagent.py ${D}/home/root/samples/cncdemo/
}

COMPATIBLE_MACHINE = "qoriq|imx"
