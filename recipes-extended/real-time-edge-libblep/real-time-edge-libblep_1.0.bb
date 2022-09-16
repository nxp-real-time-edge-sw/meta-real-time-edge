DESCRIPTION = "real-time-edge-libblep is a library for MikroBUS bee click board."
LICENSE = "GPL-2.0-only & MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=a960555d80b9e3713204934861ab9112"

SRC_URI = "file://acilib.c \
           file://LICENSE \
           file://Makefile \
           file://acilib.h \
           file://blep.c \
           file://services.h"

S = "${WORKDIR}"

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/blep_demo  ${D}/${bindir}
}
