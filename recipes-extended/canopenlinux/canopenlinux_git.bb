SUMMARY = "CANopenNode on Linux devices"
DESCRIPTION = "CANopenLinux is a CANopen stack running on Linux devices based on CANopenNode"
HOMEPAGE = "https://github.com/CANopenNode/CANopenLinux"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=3b83ef96387f14655fc854ddc3c6bd57"

DEPENDS = "libsocketcan"
RDEPENDS:${PN} = "can-utils iproute2"

SRC_URI = "gitsm://github.com/CANopenNode/CANopenLinux.git;protocol=https;branch=master \
           file://0001-Disable-the-Storage-option.patch \
           file://canopenlinux.conf \
           file://canopen-master.service \
           file://canopen-slave.service \
           file://commands.txt \
          "

SRCREV = "f1348d4072cdabea4c3435a13c721ac29ab4cc91"
PV = "4.0+git${SRCPV}"

S = "${WORKDIR}/git"

inherit systemd

SYSTEMD_SERVICE:${PN} = "canopen-master.service canopen-slave.service"
#SYSTEMD_AUTO_ENABLE:${PN} = "enable"

EXTRA_OEMAKE = " \
    'CC=${CC}' \
"

# Remap the debug path
TARGET_CC_ARCH += "${LDFLAGS}"
DEBUG_PREFIX_MAP = "-fdebug-prefix-map=${WORKDIR}=/usr/src/debug/${PN}/${EXTENDPE}${PV}-${PR}"
DEBUG_PREFIX_MAP += "-fdebug-prefix-map=${STAGING_DIR_HOST}="
DEBUG_PREFIX_MAP += "-fdebug-prefix-map=${STAGING_DIR_NATIVE}="
TARGET_CFLAGS += "${DEBUG_PREFIX_MAP}"
TARGET_CXXFLAGS += "${DEBUG_PREFIX_MAP}"

do_compile() {
    oe_runmake
    # Build cocomm tool
    cd ${S}/cocomm
    oe_runmake
}

do_install() {
    install -d ${D}${bindir}
    install -d ${D}${sysconfdir}/systemd/user
    install -d ${D}${ROOT_HOME}/canopennode

    # Install main application
    install -m 0755 ${S}/canopend ${D}${bindir}/

    # Install cocomm tool
    install -m 0755 ${S}/cocomm/cocomm ${D}${bindir}/

    # Install configuration files
    install -m 0644 ${UNPACKDIR}/canopenlinux.conf ${D}${sysconfdir}/

    # Install systemd service
    install -m 0644 ${UNPACKDIR}/canopen-master.service ${D}${sysconfdir}/systemd/user/
    install -m 0644 ${UNPACKDIR}/canopen-slave.service ${D}${sysconfdir}/systemd/user/

    # Install command list
    install -m 0644 ${UNPACKDIR}/commands.txt ${D}${ROOT_HOME}/canopennode/
}

FILES:${PN} += " \
    ${bindir}/canopend \
    ${bindir}/cocomm \
    ${sysconfdir}/canopenlinux.conf \
    ${sysconfdir}/systemd/user/canopen-master.service \
    ${sysconfdir}/systemd/user/canopen-slave.service \
    ${ROOT_HOME}/canopennode/commands.txt \
"

INSANE_SKIP:${PN} = "ldflags"
