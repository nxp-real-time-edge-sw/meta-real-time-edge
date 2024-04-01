# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for tools"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

RDEPENDS:${PN} = " \
    packagegroup-core-buildessential \
    packagegroup-core-full-cmdline \
    iperf3 e2fsprogs e2fsprogs-resize2fs \
    devmem2 \
    rteval \
"

RDEPENDS:${PN}:append = " \
    xz \
    flex \
    bison \
    openssl-dev \
"
