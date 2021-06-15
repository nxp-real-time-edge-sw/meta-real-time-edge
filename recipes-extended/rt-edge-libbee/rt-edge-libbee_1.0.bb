DESCRIPTION = "rt-edge-libbee is a library for MikroBUS bee click board."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "file://bee.c \
           file://Makefile \
           file://Init_Routines.c \
           file://Init_Routines.h \
           file://Misc_Routines.c \
           file://Misc_Routines.h \
           file://ReadWrite_Routines.c \
           file://ReadWrite_Routines.h \
           file://registers.h \
           file://Reset_Routines.h \
           file://Reset_Routines.c"

S = "${WORKDIR}"

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/bee_demo  ${D}/${bindir}
}
