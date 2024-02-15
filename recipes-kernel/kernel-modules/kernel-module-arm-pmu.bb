DESCRIPTION = "Kernel module for ARM PMU"

LICENSE = "CLOSED"
#LICENSE = "GPL-2.0-or-later"
# No need for LIC_FILES_CHKSUM valiable in this case
# External module do not have license file

inherit module

SRC_URI = "git://github.com/hemantagr/armv8_pmu_cycle_counter_el0.git;protocol=https;nobranch=1"
SRCREV = "a49de2c1bfad3c7530a6f601c5f692650bbd34f7"

S = "${WORKDIR}/git"
EXTRA_OEMAKE += "-C ${STAGING_KERNEL_BUILDDIR} M=${S}"

PACKAGE_ARCH = "${MACHINE_ARCH}"
COMPATIBLE_MACHINE = "(mx8-nxp-bsp|mx9-nxp-bsp)"

do_install() {
        install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
        install -m 0755 ${S}/pmu_el0_cycle_counter.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
}
