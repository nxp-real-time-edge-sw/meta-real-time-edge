# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for rtos industrial example"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'rtos-industrial',  \
    '${RTOS_INDUSTRIAL_INSTALL}', '', d)} \
"

RTOS_INDUSTRIAL_INSTALL = " \
    demo-hello-world \
    driver-gpio-led-output \
    freertos-hello \
    soem-gpio-pulse \
    freertos-soem-gpio-pulse \
"