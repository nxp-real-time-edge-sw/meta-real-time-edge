# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for Industrial"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-libbee', 'real-time-edge-libbee', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-libblep', 'real-time-edge-libblep', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libnfc-nci', 'libnfc-nci', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'canfestival', 'canfestival', '', d)} \
    igh-ethercat \
    libopen62541 \
    libmodbus \
"
