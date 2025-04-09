# Copyright 2024 NXP

PV = "1.5.2"
DESCRIPTION = "IgH EtherCAT Master for Linux"
HOMEPAGE = "https://etherlab.org/en/ethercat/index.php"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

SRC_URI = "git://gitlab.com/etherlab.org/ethercat.git;protocol=https;nobranch=1"
SRCREV = "418dc4b24f4eeba78f644984894b740428da3e84"

SRC_URI += "\
	file://0001-Fixed-compilation-error-for-the-EtherCat-drivers.patch \
    file://0002-fec-enect-Add-fec-and-enect-device.patch \
    file://0003-Fixed-compilation-errors-when-upgrading-to-Linux-5.x.patch \
    file://0004-Fixed-compilation-errors-because-of-linux-kernel-upg.patch \
    file://0005-Add-code-to-support-userspace-IGH-EtherCAT-for-v1.5..patch \
    file://0006-Add-code-to-support-NXP-i.MX-FEC-ethernet-port.patch \
    file://0007-yocto-not-install-header-file.patch \
"

SRC_URI += "file://libus_drv.a"

S = "${WORKDIR}/git"

PACKAGECONFIG ?= "usecat"

PACKAGECONFIG[generic] = "--enable-generic,--disable-generic,"
PACKAGECONFIG[8139too] = "--enable-8139too,--disable-8139too,"
PACKAGECONFIG[e100]    = "--enable-e100,--disable-e100,"
PACKAGECONFIG[e1000]   = "--enable-e1000,--disable-e1000,"
PACKAGECONFIG[e1000e]  = "--enable-e1000e,--disable-e1000e,"
PACKAGECONFIG[r8169]   = "--enable-r8169,--disable-r8169,"
PACKAGECONFIG[fec]     = "--enable-fec,--disable-fec,"
PACKAGECONFIG[enetc]   = "--enable-enetc,--disable-enetc,"
PACKAGECONFIG[dpaa1]   = "--enable-dpaa1,--disable-dpaa1,"
PACKAGECONFIG[backup]  = "--with-devices=2,--with-devices=1,"
PACKAGECONFIG[eoe]     = "--enable-eoe,--disable-eoe,"
PACKAGECONFIG[usecat]  = "--enable-usecat,--disable-usecat,"

do_configure[depends] += "virtual/kernel:do_shared_workdir"

inherit autotools-brokensep pkgconfig module-base

EXTRA_OECONF += " --enable-userlib=no"
EXTRA_OECONF += " --with-linux-dir=${STAGING_KERNEL_BUILDDIR}"
EXTRA_OECONF += " --with-module-dir=kernel/ethercat"

EXAMPLE_INSTALL_DIR = "examples"
FILES:${PN} += "${EXAMPLE_INSTALL_DIR}"
FILES:${PN} += "${EXAMPLE_INSTALL_DIR}/${PN}"

do_configure:prepend() {
    touch ChangeLog
}

do_compile:prepend() {
    cp ${WORKDIR}/libus_drv.a ${S}/devices/nxp_userspace/libus_drv.a
}

do_install:append() {
    install -d ${D}/${EXAMPLE_INSTALL_DIR}/${PN}
    if [ -f "${S}/examples/motor_control/.libs/ec_motor_example" ]; then
        cp examples/motor_control/.libs/ec_motor_example  ${D}/${EXAMPLE_INSTALL_DIR}/${PN}
    fi
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
