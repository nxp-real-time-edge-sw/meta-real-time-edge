# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for Real-time system"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

BAREMETAL_INSTALL = " \
    real-time-edge-baremetal \
    real-time-edge-icc \
"

RDEPENDS:${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    '${BAREMETAL_INSTALL}', '', d)} \
"
