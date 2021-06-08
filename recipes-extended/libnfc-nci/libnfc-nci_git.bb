DESCRIPTION = "Linux NFC stack for NCI based NXP NFC Controllers."
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

SRC_URI = "git://github.com/NXPNFCLinux/linux_libnfc-nci.git;protocol=https \
	file://0001-set-configuration-for-libnfc-nci.patch \
	file://0002-add-static-for-gphNxpExtns_Context.patch \
	file://0003-add-static-for-fragmentation_enabled.patch \
"

SRCREV = "8476ea1a4091e6facc37e028d8489cc2ef4741ba"
PV = "R2.4"

S = "${WORKDIR}/git"

inherit autotools-brokensep pkgconfig

EXTRA_OECONF = "LDFLAGS=-static --host=${TARGET_SYS}"
EXTRA_OEMAKE = "CCLD='${CXX}'"

do_configure() {
    ./bootstrap
    ./configure ${EXTRA_OECONF}
}

do_install() {
    install -d ${D}/${datadir}/nfc
    install -d ${D}/${sbindir}
    install ${S}/conf/*.conf ${D}/${datadir}/nfc
    install ${S}/nfcDemoApp ${D}/${sbindir}
}

INSANE_SKIP_${PN} += "ldflags"
FILES_${PN} += "${datadir}/nfc/*"
