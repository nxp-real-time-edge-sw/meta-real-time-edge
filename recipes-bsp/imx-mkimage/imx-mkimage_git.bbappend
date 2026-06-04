# Copyright 2026 NXP
# Multicore Fastboot support for imx-mkimage
#
# This bbappend integrates the i.MX93 fastboot patch into imx-mkimage.
# Fastboot allows booting multiple RTOS images on different Cortex-A cores
# with less boot time.

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
    file://0001-iMX93-add-fastboot-support.patch \
"
