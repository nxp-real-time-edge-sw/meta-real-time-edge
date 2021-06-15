DESCRIPTION = "rt-edge-libblep is a library for MikroBUS bee click board."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://acilib.c \
           file://Makefile \
           file://acilib.h \
           file://blep.c \
           file://services.h"

S = "${WORKDIR}"

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/blep_demo  ${D}/${bindir}
}
