# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for rtos industrial example"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

rpmsg-lite-examples ?= ""
rpmsg-lite-examples:append:mx8mm-nxp-bsp = " rpmsg-lite-uart-sharing-rtos"
rpmsg-lite-examples:append:mx8mm-nxp-bsp = " rpmsg-lite-str-echo-rtos"

RDEPENDS:${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'rtos-industrial',  \
    '${RTOS_INDUSTRIAL_INSTALL}', '', d)} \
"

RTOS_INDUSTRIAL_INSTALL = " \
    demo-hello-world \
    driver-gpio-led-output \
    freertos-hello \
    soem-gpio-pulse \
    freertos-soem-gpio-pulse \
    ${rpmsg-lite-examples} \
"
