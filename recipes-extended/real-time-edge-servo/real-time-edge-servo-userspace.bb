# Copyright 2024 NXP

PV = "1.0"
DESCRIPTION = "CoE test tool"
LICENSE = "GPL-2.0-only & MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=ef58f855337069acd375717db0dbbb6d"

REAL_TIME_EDGE_SERVO_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-servo.git;protocol=https"
REAL_TIME_EDGE_SERVO_BRANCH ?= "userspace"
REAL_TIME_EDGE_SERVO_SRCREV ?= "af8aac4088e55ead1aaa5f54142009837c4ffd42"

SRC_URI = "${REAL_TIME_EDGE_SERVO_SRC};branch=${REAL_TIME_EDGE_SERVO_BRANCH}"
SRCREV = "${REAL_TIME_EDGE_SERVO_SRCREV}"

S = "${WORKDIR}/git"

DEPENDS = "libxml2 igh-ethercat-userspace"
inherit pkgconfig

do_configure () {
	:
}

EXTRA_OEMAKE = " \
    PKG_CONFIG="${STAGING_BINDIR_NATIVE}/pkg-config" \
    DESTDIR="${D}"\
    HOME_DIR="${ROOT_HOME}" \
"

do_compile () {
    oe_runmake -C ${S} ${EXTRA_OEMAKE}
}

do_install() {
   oe_runmake install -C ${S} ${EXTRA_OEMAKE}
   oe_runmake install-libs -C ${S} ${EXTRA_OEMAKE}
}

FILES:${PN} += "${bindir_native}/* ${sbindir_native}/* ${libdir_native}/* ${ROOT_HOME}/*"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
