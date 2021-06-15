DESCRIPTION = "Real-time edge PRL (Pipe Rate Limiter) is a C program for pipeline manipulation."
LICENSE = "SD-3-Clause"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/BSD-3-Clause;md5=550794465ba0ec5312d6919e203a55f9"

SRC_URI = "file://prl.c \
           file://Makefile \
"

S = "${WORKDIR}"

do_install_append() {
    install -d ${D}/${bindir}
    install -m 0755 prl ${D}/${bindir}
}
