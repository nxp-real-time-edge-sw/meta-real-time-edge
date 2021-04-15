DESCRIPTION = "iomem is a tool that allows direct access to memory-mapped registers."
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

DEPENDS = "readline"

SRC_URI = "file://iomem.c \
           file://Makefile"

S = "${WORKDIR}"

# EXTRA_OEMAKE = "CFLAGS+='-I$/usr/include' \
#	        	LDFLAGS+='-L$/usr/lib' "

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/iomem  ${D}/${bindir}
}
