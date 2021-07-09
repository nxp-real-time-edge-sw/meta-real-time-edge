DESCRIPTION = "rt-edge-libbee is a library for MikroBUS bee click board."
LICENSE = "GPLv2 & MIT"
LIC_FILES_CHKSUM = "file://LICENSE;md5=a960555d80b9e3713204934861ab9112"

SRC_URI = "file://bee.c \
           file://LICENSE \
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
