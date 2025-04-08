# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for rtos industrial example"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

demo-apps ?= ""
demo-apps:append:mx8mm-nxp-bsp = " demo-hello-world "
demo-apps:append:mx8mp-nxp-bsp = " demo-hello-world "

rtos-examples ?= ""
rtos-examples:append:mx8mm-nxp-bsp = " freertos-hello "
rtos-examples:append:mx8mp-nxp-bsp = " freertos-hello "

driver-examples ?= ""
driver-examples:append:mx8mm-nxp-bsp = " driver-gpio-led-output "
driver-examples:append:mx8mp-nxp-bsp = " driver-gpio-led-output "

soem-examples ?= ""
soem-examples:append:mx8mm-nxp-bsp = " soem-gpio-pulse freertos-soem-gpio-pulse soem-servo-motor freertos-soem-servo-motor soem-servo-motor-rt1180 freertos-soem-servo-motor-rt1180 "
soem-examples:append:mx8mp-nxp-bsp = " soem-gpio-pulse freertos-soem-gpio-pulse soem-servo-motor freertos-soem-servo-motor soem-servo-motor-rt1180 freertos-soem-servo-motor-rt1180 "
soem-examples:append:imx93evk = " soem-servo-motor soem-servo-motor-rt1180 "

rpmsg-lite-examples ?= ""
rpmsg-lite-examples:append:imx93-9x9-lpddr4-qsb = " rpmsg-lite-uart-sharing-rtos-mcimx93qsb "

uart-examples ?= ""
uart-examples:append:mx8mm-nxp-bsp = " 9bit-iuart-interrupt-transfer 9bit-iuart-polling "

heterogeneous-multicore-examples ?= ""
heterogeneous-multicore-examples:append:mx8mm-nxp-bsp = " hello-world-ca-zephyr hello-world-ca hello-world-cm rpmsg-uart-sharing-cm virtio-perf-ca virtio-perf-cm hmc-tools "
heterogeneous-multicore-examples:append:mx8mm-nxp-bsp = " virtio-net-backend-ca virtio-net-backend-cm "
heterogeneous-multicore-examples:append:mx8mm-nxp-bsp = " lwip-ping-ca rpmsg-str-echo-ca rpmsg-str-echo-cm rpmsg-str-echo-8m-cm rt-latency-ca rt-latency-ca-zephyr "
heterogeneous-multicore-examples:append:mx8mm-nxp-bsp = " soem-digital-io-ca soem-servo-ca soem-servo-rt1180-ca "
heterogeneous-multicore-examples:append:mx8mp-nxp-bsp = " hello-world-ca-zephyr hello-world-ca hello-world-cm virtio-perf-ca virtio-perf-cm virtio-net-backend-ca virtio-net-backend-cm hmc-tools "
heterogeneous-multicore-examples:append:mx8mp-nxp-bsp = " lwip-ping-ca rpmsg-str-echo-ca rpmsg-str-echo-cm "
heterogeneous-multicore-examples:append:mx8mp-nxp-bsp = " rpmsg-pingpong-master-ca rpmsg-pingpong-remote-ca "
heterogeneous-multicore-examples:append:mx8mp-nxp-bsp = " rpmsg-perf-ca rpmsg-perf-cm rt-latency-ca rt-latency-ca-zephyr "
heterogeneous-multicore-examples:append:mx8mp-nxp-bsp = " soem-digital-io-ca soem-servo-ca soem-servo-rt1180-ca "
heterogeneous-multicore-examples:append:imx93evk = " hello-world-ca-zephyr hello-world-ca hello-world-cm lwip-ping-ca rpmsg-uart-sharing-cm virtio-net-backend-ca virtio-net-backend-cm hmc-tools "
heterogeneous-multicore-examples:append:imx93evk = " rt-latency-ca rt-latency-ca-zephyr rpmsg-str-echo-ca "
heterogeneous-multicore-examples:append:imx93evk = " soem-digital-io-ca soem-servo-ca soem-servo-rt1180-ca "
heterogeneous-multicore-examples:append:imx93-14x14-lpddr4x-evk = " hello-world-ca-zephyr hello-world-ca hello-world-cm lwip-ping-ca rpmsg-uart-sharing-cm virtio-net-backend-ca virtio-net-backend-cm hmc-tools "
heterogeneous-multicore-examples:append:imx93-14x14-lpddr4x-evk = " rt-latency-ca rt-latency-ca-zephyr rpmsg-str-echo-ca "
heterogeneous-multicore-examples:append:imx95-15x15-lpddr4x-evk = " hello-world-ca-zephyr hello-world-ca rt-latency-ca rt-latency-ca-zephyr hmc-tools "
heterogeneous-multicore-examples:append:imx95-19x19-lpddr5-evk = " hello-world-ca-zephyr hello-world-ca rt-latency-ca rt-latency-ca-zephyr hmc-tools "
heterogeneous-multicore-examples:append:imx91-9x9-lpddr4-qsb = " hello-world-ca-zephyr hello-world-ca rt-latency-ca rt-latency-ca-zephyr "
heterogeneous-multicore-examples:append:imx91-11x11-lpddr4-evk = " hello-world-ca-zephyr hello-world-ca rt-latency-ca rt-latency-ca-zephyr "

heterogeneous-multi-soc-examples ?= ""
heterogeneous-multi-soc-examples:append:mx8mp-nxp-bsp = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous-multi-soc-examples:append:imx93evk = " dsa-switch-evkmimxrt1180-cm33 "
heterogeneous-multi-soc-examples:append:mx943-nxp-bsp = " dsa-switch-evkmimxrt1180-cm33 "

RDEPENDS:${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'rtos-industrial',  \
    '${RTOS_INDUSTRIAL_INSTALL}', '', d)} \
"

RTOS_INDUSTRIAL_INSTALL = " \
    ${demo-apps} \
    ${rtos-examples} \
    ${driver-examples} \
    ${soem-examples} \
    ${rpmsg-lite-examples} \
    ${uart-examples} \
    ${heterogeneous-multicore-examples} \
    ${heterogeneous-multi-soc-examples} \
"
