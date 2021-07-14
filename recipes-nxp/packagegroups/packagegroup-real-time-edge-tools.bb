# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for tools"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

RDEPENDS_${PN} = " \
    iperf3 e2fsprogs e2fsprogs-resize2fs \
    devmem2 \
    ${@bb.utils.contains('DISTRO_FEATURES', 'feedgnuplot', 'feedgnuplot', '', d)} \
"
