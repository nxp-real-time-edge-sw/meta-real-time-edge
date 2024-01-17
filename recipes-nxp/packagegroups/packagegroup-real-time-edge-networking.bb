# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "Real-time Edge Package group for Networking"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

PACKAGES = "${PN}"

tsntoolimage ?= ""
tsntoolimage:ls1028a = "tsntool"

genavbtsnimage ?= ""
genavbtsnimage:ls1028ardb = "genavb-tsn"
genavbtsnimage:imx8mp-lpddr4-evk = "genavb-tsn"
genavbtsnimage:imx93evk = "genavb-tsn"
genavbtsnimage:imx93-9x9-lpddr4-qsb = "genavb-tsn"
genavbtsnimage:imx8dxlb0-lpddr4-evk = "genavb-tsn"

cantools ?=""
cantools:ls1021aiot = "libsocketcan can-utils"
cantools:ls1028ardb = "libsocketcan can-utils"

virtioperftool ?=""
virtioperftool:imx8mm-lpddr4-evk = "virtio-perf-tool"

RDEPENDS:${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-sysrepo', 'real-time-edge-sysrepo', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'tsn-scripts', 'tsn-scripts', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-nodejs-lbt', 'real-time-edge-nodejs-lbt', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'real-time-edge-prl', 'real-time-edge-prl', '', d)} \
    linuxptp \
    ${tsntoolimage} \
    lldpd \
    avahi-daemon avahi-utils \
    python3-websockets \
    rt-tests \
    iproute2-tc \
    iproute2-devlink \
    tcpdump \
    ${genavbtsnimage} \
    ethtool \
    ebtables \
    iproute2 \
    ${cantools} \
    openssh-sftp-server \
    openssh-keygen \
    pktgen-scripts \
    ${virtioperftool} \
"
