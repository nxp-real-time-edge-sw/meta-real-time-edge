# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "NXP Package group for RT Edge"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

tsntoolimage ?= ""
tsntoolimage_ls1028a = "tsntool"

RDEPENDS_${PN} = " \
    iomem \
    ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    'packagegroup-baremetal', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'jailhouse-rt-edge',  \
    'jailhouse-rt-edge', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libbee', 'libbee', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libblep', 'libblep', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libnfc-nci', 'libnfc-nci', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'sysrepo-tsn', 'sysrepo-tsn', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'tsn-scripts', 'tsn-scripts', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'canfestival', 'canfestival', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'nodejs-lbt', 'nodejs-lbt', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'feedgnuplot', 'feedgnuplot', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'prl', 'prl', '', d)} \
    linuxptp \
    igh-ethercat \
    ${tsntoolimage} \
    lldpd \
    avahi-daemon avahi-utils \
    qoriq-servo \
    libopen62541 \
    libmodbus \
    python3-websockets \
"
