DESCRIPTION = "Linux NFC stack for NCI based NXP NFC Controllers."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRCBRANCH ?= "master"
SRC_URI = "git://github.com/NXPNFCLinux/linux_libnfc-nci.git;protocol=https;branch=${SRCBRANCH} \
    file://0001-set-configuration-for-libnfc-nci.patch \
    file://0002-add-static-for-gphNxpExtns_Context.patch \
    file://0003-add-static-for-fragmentation_enabled.patch \
    file://0004-demoapp-main.c-fix-compile-issue.patch \
"

SRCREV = "8476ea1a4091e6facc37e028d8489cc2ef4741ba"
PV = "R2.4"

S = "${WORKDIR}/git"

inherit autotools-brokensep pkgconfig

EXTRA_OECONF = "LDFLAGS=-static --host=${TARGET_SYS}"
EXTRA_OEMAKE = "CFLAGS='-D_GNU_SOURCE -Wno-implicit-function-declaration \
                -Wno-implicit-int -Wno-int-conversion -Wno-return-mismatch \
                -Wno-incompatible-pointer-types' CCLD='${CXX}'"

do_configure() {
    ./bootstrap
    ./configure ${EXTRA_OECONF}
}

do_install() {
    install -d ${D}/data/nfc
    install -d ${D}/${sbindir}
    install ${S}/conf/*.conf ${D}/data/nfc
    install ${S}/nfcDemoApp ${D}/${sbindir}
}

INSANE_SKIP:${PN} += "ldflags"
FILES:${PN} += "data/nfc/*"
