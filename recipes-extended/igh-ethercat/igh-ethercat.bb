# Copyright 2021 NXP

PV = "1.5.2"
DESCRIPTION = "IgH EtherCAT Master for Linux"
HOMEPAGE = "https://etherlab.org/en/ethercat/index.php"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=59530bdf33659b29e73d4adb9f9f6552"

SRC_URI = "http://etherlab.org/download/ethercat/ethercat-${PV}.tar.bz2"
SRC_URI[md5sum] = "6b4001f8d975865d74a0b108b3bdda3d"

SRC_URI += "\
    file://0001-Fixed-compilation-error-for-the-EtherCat-drivers.patch \
    file://0002-Modify-the-example-code.patch \
    file://0003-Fixed-compilation-error-for-the-IGH-EtherCAT.patch \
    file://0004-replace-the-init_timer-with-timer_setup-function.patch \
    file://0005-igh-ethernet-change-the-type-of-eccdev_vma_fault-to-.patch \
    file://0006-examples-user-simplify-the-Igh-test-case.patch \
    file://0007-configure-Fix-the-subdir-objects-error.patch \
    file://0008-master-master-fix-the-issue-of-sched_setscheduler-un.patch \
    file://0009-device-fec-Add-fec-device.patch \
    file://0010-device-fec-add-fec-support-on-conf-script.patch \
    file://0011-Fix-ethercat-tool-compilation.patch \
    file://0012-enect-Add-enetc-souce-code-based-on-linux-4.14.patch \
    file://0013-ec_enetc-Add-ec_enetc-native-driver-based-on-linux-4.patch \
    file://0014-enetc-port-enetc-vf-driver-to-linux-5.10.patch \
    file://0015-enetc-add-pf-device-support-for-linux-5.10.patch \
    file://0016-enetc-adjust-the-link-status-dynamically-for-ec_mast.patch \
    file://0017-enetc-mq-is-useless-to-ndev-struct.patch \
    file://0018-enetc-add-enetc-support-on-scripts.patch \
    file://0001-Add-support-for-NXP-DPAA1-ethercat-port.patch \
    file://0002-igh_ethercat-fix-calltrace-issue-caused-by-ethercatc.patch \
    file://0001-enetc-fix-the-compile-errors-when-upgrading-to-Linux.patch \
    file://0001-dpaa1-Add-module-license.patch \
"
S = "${WORKDIR}/ethercat-${PV}"

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

do_configure[depends] += "virtual/kernel:do_shared_workdir"

inherit autotools-brokensep pkgconfig module-base

EXTRA_OECONF += "--with-linux-dir=${STAGING_KERNEL_BUILDDIR}"
EXTRA_OECONF += "--with-module-dir=kernel/ethercat"

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
