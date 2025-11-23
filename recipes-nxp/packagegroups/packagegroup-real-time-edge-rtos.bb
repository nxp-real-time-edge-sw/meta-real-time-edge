# Copyright 2021, 2025 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for rtos industrial example"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

demo_apps ?= ""
demo_apps:append:imx8mm-lpddr4-evk = " demo-hello-world "
demo_apps:append:imx8mp-lpddr4-evk = " demo-hello-world "
demo_apps:append:imx93-11x11-lpddr4x-evk = " demo-hello-world "
demo_apps:append:imx943-19x19-lpddr4-evk = " demo-hello-world "
demo_apps:append:imx943-19x19-lpddr5-evk = " demo-hello-world "
demo_apps:append:imx95-15x15-lpddr4x-evk = " demo-hello-world "
demo_apps:append:imx95-19x19-lpddr5-evk = " demo-hello-world "

freertos_examples ?= ""
freertos_examples:append:imx8mm-lpddr4-evk = " freertos-hello "
freertos_examples:append:imx8mp-lpddr4-evk = " freertos-hello "
freertos_examples:append:imx93-11x11-lpddr4x-evk = " freertos-hello "
freertos_examples:append:imx943-19x19-lpddr4-evk = " freertos-hello "
freertos_examples:append:imx943-19x19-lpddr5-evk = " freertos-hello "
freertos_examples:append:imx95-15x15-lpddr4x-evk = " freertos-hello "
freertos_examples:append:imx95-19x19-lpddr5-evk = " freertos-hello "

driver_examples ?= ""
driver_examples:append:imx8mm-lpddr4-evk = " igpio-led-output "
driver_examples:append:imx8mp-lpddr4-evk = " igpio-led-output "
driver_examples:append:imx93-11x11-lpddr4x-evk = " rgpio-led-output "
driver_examples:append:imx943-19x19-lpddr4-evk = " rgpio-led-output "
driver_examples:append:imx943-19x19-lpddr5-evk = " rgpio-led-output "
driver_examples:append:imx95-15x15-lpddr4x-evk = " rgpio-led-output "
driver_examples:append:imx95-19x19-lpddr5-evk = " rgpio-led-output "

soem_examples ?= ""
soem_examples:append:imx8mm-lpddr4-evk = " \
    soem-gpio-pulse-bm soem-gpio-pulse-freertos \
    soem-servo-motor-bm soem-servo-motor-freertos \
    soem-servo-motor-rt1180-bm soem-servo-motor-rt1180-freertos \
"
soem_examples:append:imx8mp-lpddr4-evk = " \
    soem-gpio-pulse-bm soem-gpio-pulse-freertos \
    soem-servo-motor-bm soem-servo-motor-freertos \
    soem-servo-motor-rt1180-bm soem-servo-motor-rt1180-freertos \
"
soem_examples:append:imx93evk = " \
    soem-servo-motor-bm \
    soem-servo-motor-rt1180-bm \
"
soem_examples:append:imx943-19x19-lpddr4-evk = " \
    soem-gpio-pulse-netc-bm \
    soem-servo-motor-bm \
    soem-servo-motor-rt1180-bm \
"
soem_examples:append:imx943-19x19-lpddr5-evk = " \
    soem-gpio-pulse-netc-bm \
    soem-servo-motor-bm \
    soem-servo-motor-rt1180-bm \
"

uart_examples ?= ""
uart_examples:append:imx8mm-lpddr4-evk = " \
    iuart-9bit-interrupt-transfer \
    iuart-9bit-polling \
"

digital_encoder_examples ?= ""
digital_encoder_examples:append:imx943-19x19-lpddr4-evk = " \
    biss \
    endat2p2 \
    endat3 \
    hiperface \
    a-format-interrupt-transfer \
    a-format-polling-transfer \
    a-format-sync-transfer \
    t-format-interrupt-transfer \
    t-format-polling-transfer \
    t-format-sync-transfer \
"
digital_encoder_examples:append:imx943-19x19-lpddr5-evk = " \
    biss \
    endat2p2 \
    endat3 \
    hiperface \
    a-format-interrupt-transfer \
    a-format-polling-transfer \
    a-format-sync-transfer \
    t-format-interrupt-transfer \
    t-format-polling-transfer \
    t-format-sync-transfer \
"

heterogeneous_multicore_examples ?= ""

heterogeneous_multicore_examples:append:imx8mm-lpddr4-evk = " \
    hello-world \
    rpmsg-uart-sharing \
    rpmsg-str-echo \
    rpmsg-str-echo-8m-buf \
    virtio-perf \
    virtio-net-backend \
    lwip-ping \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx8mp-lpddr4-evk = " \
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

heterogeneous_multicore_examples:append:imx93-9x9-lpddr4-qsb = " \
    rpmsg-uart-sharing \
"

heterogeneous_multicore_examples:append:imx943-19x19-lpddr4-evk = " \
    hello-world \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx943-19x19-lpddr5-evk = " \
    hello-world \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx943-15x15-lpddr4-evk = " \
    hello-world \
    rt-latency \
    hmc-tools \
"

heterogeneous_multicore_examples:append:imx95-15x15-lpddr4x-evk = " \
    hello-world \
    rpmsg-str-echo \
    rpmsg-pingpong \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
"

heterogeneous_multicore_examples:append:imx95-19x19-lpddr5-evk = " \
    hello-world \
    netc-share \
    rpmsg-str-echo \
    rpmsg-pingpong \
    rt-latency \
    hmc-tools \
    soem-digital-io \
    soem-servo \
    soem-servo-rt1180 \
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
heterogeneous_multi_soc_examples:append:imx8mp-lpddr4-evk = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous_multi_soc_examples:append:imx93evk = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous_multi_soc_examples:append:imx943-19x19-lpddr4-evk = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous_multi_soc_examples:append:imx943-19x19-lpddr5-evk = " dsa-switch-evkmimxrt1180-cm33 "

RDEPENDS:${PN} = " \
    ${RTOS_INDUSTRIAL_INSTALL} \
"

RTOS_INDUSTRIAL_INSTALL = " \
    ${demo_apps} \
    ${freertos_examples} \
    ${driver_examples} \
    ${soem_examples} \
    ${uart_examples} \
    ${digital_encoder_examples} \
    ${heterogeneous_multicore_examples} \
    ${heterogeneous_multi_soc_examples} \
"
