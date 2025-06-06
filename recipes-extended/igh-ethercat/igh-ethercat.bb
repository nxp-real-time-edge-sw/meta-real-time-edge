# Copyright 2021 NXP

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
    file://0001-enetc4-port-enetc4.patch \
    file://0002-fec-fix-compile-error-because-of-kernel-upgrade-to-6.patch \
	file://0001-enetc4-ec_enetc4-and-ec_master-modules-worked.patch \
"
S = "${WORKDIR}/git"

ERROR_QA:remove = "buildpaths"
WARN_QA:append = " buildpaths"

PACKAGECONFIG ??= "generic"

PACKAGECONFIG[generic] = "--enable-generic,--disable-generic,"
PACKAGECONFIG[8139too] = "--enable-8139too,--disable-8139too,"
PACKAGECONFIG[e100]    = "--enable-e100,--disable-e100,"
PACKAGECONFIG[e1000]   = "--enable-e1000,--disable-e1000,"
PACKAGECONFIG[e1000e]  = "--enable-e1000e,--disable-e1000e,"
PACKAGECONFIG[r8169]   = "--enable-r8169,--disable-r8169,"
PACKAGECONFIG[fec]     = "--enable-fec,--disable-fec,"
PACKAGECONFIG[enetc]   = "--enable-enetc,--disable-enetc,"
PACKAGECONFIG[dpaa1]   = "--enable-dpaa1,--disable-dpaa1,"
PACKAGECONFIG[backup]   = "--with-devices=2,--with-devices=1,"
PACKAGECONFIG[eoe]     = "--enable-eoe,--disable-eoe,"

do_configure[depends] += "virtual/kernel:do_shared_workdir"

inherit autotools-brokensep pkgconfig module-base

EXTRA_OECONF += "--with-linux-dir=${STAGING_KERNEL_BUILDDIR}"
EXTRA_OECONF += "--with-module-dir=kernel/ethercat"

do_configure:prepend() {
    touch ChangeLog
}

do_compile:append() {
	oe_runmake modules
}

inherit systemd
do_install:append() {
	oe_runmake MODLIB=${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION} modules_install
	rm -rf ${D}${sysconfdir}/init.d/ethercat
	if [ ! -z "${PACKAGECONFIG}" ]; then
		if [ ! -d ${D}${sysconfdir}/modprobe.d ]; then
			mkdir ${D}${sysconfdir}/modprobe.d
		fi
		if [ ! -e ${D}${sysconfdir}/modprobe.d/igh-ethercat.conf ]; then
			touch ${D}${sysconfdir}/modprobe.d/igh-ethercat.conf
		fi
		for var in ${PACKAGECONFIG}
		do
			echo "blacklist ec_${var}" >> ${D}${sysconfdir}/modprobe.d/igh-ethercat.conf
		done
	fi
}

FILES:${PN} += "${nonarch_base_libdir}/modules/${KERNEL_VERSION}/* ${systemd_system_unitdir}/ethercat.service ${bindir_native}/* ${sbindir_native}/* ${libdir_native}/* ${sysconfdir_native}/*"
SYSTEMD_SERVICE:ethercat = "ethercat.service"
SYSTEMD_AUTO_ENABLE = "disable"

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
