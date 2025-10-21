# Copyright 2024 NXP

DESCRIPTION = "IgH EtherCAT Master for Linux"
HOMEPAGE = "https://etherlab.org/en/ethercat/index.php"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

IGH_USERSPACE_SRC ?= "git://gitlab.com/etherlab.org/ethercat.git;protocol=https"
IGH_USERSPACE_BRANCH ?= "stable-1.6"
IGH_USERSPACE_SRCREV ?= "703b6117288bbe4e6f74ecb4a6b8ef13f390b898"

SRC_URI = "${IGH_USERSPACE_SRC};branch=${IGH_USERSPACE_BRANCH}"
SRCREV = "${IGH_USERSPACE_SRCREV}"
PV = "1.6.4"

SRC_URI += " \
    file://0001-Add-code-to-support-userspace-IGH-EtherCAT-for-v1.6.patch \
    file://0002-Add-code-to-support-NXP-i.MX-FEC-ethernet-port.patch \
    file://0003-yocto-change-header-name-and-not-install-libethercat.patch \
    file://0004-Update-doc.patch \
    file://0005-Allocate-non-cached-bd_buf-in-kernel-for-i.MX943-and.patch \
    file://0006-Makefile.kbuild-Handle-empty-LINUX_SOURCE_DIR-in-kbu.patch \
    file://0007-devices-nxp_userspace-Add-ifndef-guard-for-ETH_P_ETH.patch \
    file://libus_drv.a;subdir=git/devices/nxp_userspace/ \
"

S = "${WORKDIR}/git"
B = "${S}"

PACKAGECONFIG ?= "usecat"

PACKAGECONFIG[generic] = "--enable-generic,--disable-generic,"
PACKAGECONFIG[8139too] = "--enable-8139too,--disable-8139too,"
PACKAGECONFIG[e100]    = "--enable-e100,--disable-e100,"
PACKAGECONFIG[e1000]   = "--enable-e1000,--disable-e1000,"
PACKAGECONFIG[e1000e]  = "--enable-e1000e,--disable-e1000e,"
PACKAGECONFIG[r8169]   = "--enable-r8169,--disable-r8169,"
PACKAGECONFIG[backup]  = "--with-devices=2,--with-devices=1,"
PACKAGECONFIG[eoe]     = "--enable-eoe,--disable-eoe,"
PACKAGECONFIG[usecat]  = "--enable-usecat,--disable-usecat,"

inherit autotools-brokensep pkgconfig module-base

EXTRA_OECONF += " \
    --enable-kernel=no \
    --enable-tool=yes \
"

EXAMPLE_INSTALL_DIR = "examples"
FILES:${PN} += "${EXAMPLE_INSTALL_DIR}"
FILES:${PN} += "${EXAMPLE_INSTALL_DIR}/${PN}"

do_configure:prepend() {
    cd ${S}
    ./bootstrap
}

# do_compile:prepend() {
#    cp ${UNPACKDIR}/libus_drv.a ${S}/devices/nxp_userspace/
# }

do_install:append() {
    install -d ${D}/${EXAMPLE_INSTALL_DIR}/${PN}
    if [ -f "${S}/examples/motor_control/.libs/ec_motor_example" ]; then
        cp ${S}/examples/motor_control/.libs/ec_motor_example  ${D}/${EXAMPLE_INSTALL_DIR}/${PN}
    fi
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
