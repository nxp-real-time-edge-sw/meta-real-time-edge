DESCRIPTION = "Real-time edge PRL (Pipe Rate Limiter) is a C program for pipeline manipulation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=8636bd68fc00cc6a3809b7b58b45f982"

SRC_URI = "file://LICENSE \
           file://prl.c \
           file://Makefile \
"

S = "${WORKDIR}"

do_install:append() {
    install -d ${D}/${bindir}
    install -m 0755 prl ${D}/${bindir}
}
