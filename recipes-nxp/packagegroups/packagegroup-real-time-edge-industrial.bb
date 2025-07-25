# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for Industrial"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

userscapce-servo ?= ""
userscapce-servo:imx8mp-lpddr4-evk = "igh-ethercat-userspace"
userscapce-servo:imx8mm-lpddr4-evk = "igh-ethercat-userspace"
userscapce-servo:imx93evk = "igh-ethercat-userspace"
userscapce-servo:mx943-nxp-bsp = "igh-ethercat-userspace"
userscapce-servo:mx95-nxp-bsp = "igh-ethercat-userspace"

RDEPENDS:${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-libbee', 'real-time-edge-libbee', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-libblep', 'real-time-edge-libblep', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libnfc-nci', 'libnfc-nci', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'canfestival', 'canfestival', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'modbus-simulator', 'modbus-simulator', '', d)} \
    igh-ethercat \
    real-time-edge-servo \
    ${userscapce-servo} \
    libopen62541 \
    libmodbus \
"
