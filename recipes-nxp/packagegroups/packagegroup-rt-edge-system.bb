# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "RT Edge Package group for RT system"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

BAREMETAL_INSTALL = " \
    u-boot-baremetal \
    icc \
"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    '${BAREMETAL_INSTALL}', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'jailhouse-rt-edge',  \
    'jailhouse-rt-edge', '', d)} \
"