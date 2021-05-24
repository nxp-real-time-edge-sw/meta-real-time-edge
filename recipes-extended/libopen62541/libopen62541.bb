# Copyright 2020-2021 NXP

SUMMARY = "OPC UA implementation"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

SRC_URI = "gitsm://github.com/open62541/open62541.git;protocol=https"

# Modify these as desired
PV = "v1.2.2"
SRCREV = "ecf5a703785877a8719a0cda863a98455f7d5d12"

S = "${WORKDIR}/git"

DEPENDS = "openssl"

do_install_append () {
	install -d ${D}${ROOT_HOME}/open62541_example/
	install -m 644 ${B}/bin/examples/* ${D}${ROOT_HOME}/open62541_example/
}

FILES_${PN} += "${bindir_native}/* ${datadir_native}/open62541/* ${libdir_native}/* ${ROOT_HOME}/*"

inherit cmake python3native

