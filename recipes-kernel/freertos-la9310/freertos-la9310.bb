# Copyright 2023-2024 NXP

require la93xx-sdk-src.inc
require la93xx-sdk-common.inc

inherit deploy

do_configure[noexec] = "1"
# do_rootfs[noexec] = "1"
# do_package[noexec] = "1"
do_package_qa[noexec] = "1"
# do_packagedata[noexec] = "1"
# do_package_write_ipk[noexec] = "1"
# do_package_write_deb[noexec] = "1"
# do_package_write_rpm[noexec] = "1"

INHIBIT_DEFAULT_DEPS = "1"
# INHIBIT_SYSROOT_STRIP = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

DEPENDS = "gcc-arm-none-eabi-native"
ARMGCC_DIR ?= "${WORKDIR}/recipe-sysroot-native${libexecdir}/gcc-arm-none-eabi"

LA93XX_SRC_DIR = "${S}/Demo/CORTEX_M4_NXP_LA9310_GCC"

do_compile() {
    export ARMGCC_DIR="${ARMGCC_DIR}"
    export LA9310_COMMON_HEADERS="${S}/common_headers"
    echo "Start to compile la93xx with ${ARMGCC_DIR}"
    cd ${LA93XX_SRC_DIR}

    chmod u+x *.sh
    ./clean.sh
if [ imx8mp-rfnm ]; then
    ./build_release.sh -m pcie -t rfnm -b Release
else
    ./build_release.sh -m pcie -t nlm -b Release
fi

}

FILES:${PN}= "/lib"

do_install () {
        install -d ${D}${nonarch_base_libdir}/firmware
        install -m0755 ${S}/Demo/CORTEX_M4_NXP_LA9310_GCC/release/* ${D}${nonarch_base_libdir}/firmware
        install -m0755 ${S}/firmware/*.eld ${D}${nonarch_base_libdir}/firmware
}

