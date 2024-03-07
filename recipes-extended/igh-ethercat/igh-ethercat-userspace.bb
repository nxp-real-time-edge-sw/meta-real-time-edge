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
    file://0001-fec-fix-the-compile-error-when-upgrading-to-Linux-5..patch \
    file://0001-enetc-upgrade-to-be-compatible-to-Linux-5.15.52.patch \
    file://0001-device-fec-rgmii_rxc_dly-is-set-to-true-on-i.mx8dxl.patch \
    file://0001-Fix-igh-ethercat-compile-failure-issue.patch \
    file://0001-igh-fix-igh-compile-failed-on-ls1028.patch \
    file://0001-Native-Driver-fix-ec_fec-kernel-module-insmod-failur.patch \
    file://0001-net-enetc-use-lynx_pcs_create_mdiodev.patch \
    file://0002-net-enetc-integrate-SerDes-phys-with-lynx-pcs.patch \
    file://0001-Add-code-to-support-userspace-IGH-EtherCAT-for-v1.5..patch \
    file://0002-Add-code-to-support-NXP-i.MX-FEC-ethernet-port.patch \
    file://0003-Fix-the-compile-error.patch \
    file://0001-Fix-the-issue-that-shm_get-key-is-using-relative-pat.patch \
    file://0002-Update-Readme_For_User_Space_IGH_EtherCAT.txt.patch \
    file://0003-Modify-ecrt.h-for-real-time-edge-servo-and-user-appl.patch \
"

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

EXTRA_OECONF += " --enable-userlib=no --enable-tool=no"
EXTRA_OECONF += " --with-linux-dir=${STAGING_KERNEL_BUILDDIR}"
EXTRA_OECONF += " --with-module-dir=kernel/ethercat"

do_configure:prepend() {
    touch ChangeLog
}

INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
