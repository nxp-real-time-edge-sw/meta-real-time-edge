# Copyright 2021, 2025 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for rtos industrial example"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

demo_apps ?= ""
demo_apps:append:mx8mm-nxp-bsp = " demo-hello-world "
demo_apps:append:mx8mp-nxp-bsp = " demo-hello-world "
demo_apps:append:mx93-nxp-bsp = " demo-hello-world "
demo_apps:append:mx943-nxp-bsp = " demo-hello-world "
demo_apps:append:mx95-nxp-bsp = " demo-hello-world "

freertos_examples ?= ""
freertos_examples:append:mx8mm-nxp-bsp = " freertos-hello "
freertos_examples:append:mx8mp-nxp-bsp = " freertos-hello "
freertos_examples:append:mx93-nxp-bsp = " freertos-hello "
freertos_examples:append:mx943-nxp-bsp = " freertos-hello "
freertos_examples:append:mx95-nxp-bsp = " freertos-hello "

driver_examples ?= ""
driver_examples:append:mx8mm-nxp-bsp = " igpio-led-output "
driver_examples:append:mx8mp-nxp-bsp = " igpio-led-output "
driver_examples:append:mx93-nxp-bsp = " rgpio-led-output "
driver_examples:append:mx943-nxp-bsp = " rgpio-led-output "
driver_examples:append:mx95-nxp-bsp = " rgpio-led-output "

soem_examples ?= ""
# soem_examples:append:mx8mm-nxp-bsp = " soem-gpio-pulse freertos-soem-gpio-pulse soem-servo-motor freertos-soem-servo-motor soem-servo-motor-rt1180 freertos-soem-servo-motor-rt1180 "
# soem_examples:append:mx8mp-nxp-bsp = " soem-gpio-pulse freertos-soem-gpio-pulse soem-servo-motor freertos-soem-servo-motor soem-servo-motor-rt1180 freertos-soem-servo-motor-rt1180 "
# soem_examples:append:imx93evk = " soem-servo-motor soem-servo-motor-rt1180 "

rpmsg_lite_examples ?= ""
# rpmsg_lite_examples:append:imx93-9x9-lpddr4-qsb = " rpmsg-lite-uart-sharing-rtos-mcimx93qsb "

uart_examples ?= ""
# uart_examples:append:mx8mm-nxp-bsp = " 9bit-iuart-interrupt-transfer 9bit-iuart-polling "

heterogeneous_multicore_examples ?= ""

heterogeneous_multicore_examples:append:mx8mm-nxp-bsp = " \
    hello-world \
    rpmsg-uart-sharing \
    rpmsg-str-echo \
    virtio-perf \
    virtio-net-backend \
    lwip-ping \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:mx8mp-nxp-bsp = " \
    hello-world \
    rpmsg-str-echo \
    rpmsg-pingpong \
    rpmsg-perf \
    virtio-perf \
    virtio-net-backend \
    lwip-ping \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx93evk = " \
    hello-world \
    rpmsg-uart-sharing \
    rpmsg-str-echo \
    rpmsg-pingpong \
    virtio-net-backend \
    lwip-ping \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx93-14x14-lpddr4x-evk = " \
    hello-world \
    rpmsg-uart-sharing \
    rpmsg-str-echo \
    rpmsg-pingpong \
    virtio-net-backend \
    lwip-ping \
    rt-latency \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx943-19x19-lpddr4-evk = " \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx943-19x19-lpddr5-evk = " \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx95-15x15-lpddr4x-evk = " \
    hello-world \
    rpmsg-str-echo \
    rt-latency \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx95-19x19-lpddr5-evk = " \
    hello-world \
    rpmsg-str-echo \
    rt-latency \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx91-9x9-lpddr4-qsb = " \
    hello-world \
    rt-latency \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx91-11x11-lpddr4-evk = " \
    hello-world \
    rt-latency \
    hmc-tools \
"

heterogeneous_multi_soc_examples ?= ""
heterogeneous_multi_soc_examples:append:mx8mp-nxp-bsp = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous_multi_soc_examples:append:imx93evk = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous_multi_soc_examples:append:mx943-nxp-bsp = " dsa-switch-evkmimxrt1180-cm33 "

RDEPENDS:${PN} = " \
    ${RTOS_INDUSTRIAL_INSTALL} \
"

RTOS_INDUSTRIAL_INSTALL = " \
    ${demo_apps} \
    ${freertos_examples} \
    ${driver_examples} \
    ${soem_examples} \
    ${rpmsg_lite_examples} \
    ${uart_examples} \
    ${heterogeneous_multicore_examples} \
    ${heterogeneous_multi_soc_examples} \
"
