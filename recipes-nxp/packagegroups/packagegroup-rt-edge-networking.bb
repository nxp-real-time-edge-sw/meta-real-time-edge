# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

SUMMARY = "RT Edge Package group for Networking"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COREBASE}/meta/COPYING.MIT;md5=3da9cfbcb788c80a0384361b4de20420"

inherit packagegroup

PACKAGE_ARCH = "${MACHINE_ARCH}"
PACKAGES = "${PN}"

tsntoolimage ?= ""
tsntoolimage_ls1028a = "tsntool"

genavbtsnimage ?= ""
genavbtsnimage_ls1028ardb = "genavb-tsn"
genavbtsnimage_imx8mpevk = "genavb-tsn"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'sysrepo-tsn', 'sysrepo-tsn', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'tsn-scripts', 'tsn-scripts', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'canfestival', 'canfestival', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'rt-edge-nodejs-lbt', 'rt-edge-nodejs-lbt', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'prl', 'prl', '', d)} \
    linuxptp \
    ${tsntoolimage} \
    lldpd \
    avahi-daemon avahi-utils \
    nxp-servo \
    python3-websockets \
    rt-tests \
    iproute2-tc \
    tcpdump \
    ${genavbtsnimage} \
    ethtool \
    iproute2 \
    openssh-sftp-server \
    openssh-keygen \
"
