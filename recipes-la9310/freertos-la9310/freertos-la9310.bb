# Copyright 2023-2024 NXP

require la93xx-sdk-common.inc
require ${BSPDIR}/sources/meta-real-time-edge/recipes-kernel/include/la93xx-repo.inc


LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

# This file might be included from other places (like other layers) and not
# having an explicit path to the patches directory, will make bitbake fail to
# find the patch(es) in SRC_URI.
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRCREV_FORMAT = "la93xx-sdk"

SRC_URI = "\
    ${SRC_LA9310_FRTOS_URI};name=freertos_repo \
    ${SRC_LA9310_FW_URI};name=firmware_repo;destsuffix=git/firmware \
    "

S = "${WORKDIR}/git"

# Default to a stable version

PACKAGES = "${PN}"

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
INHIBIT_SYSROOT_STRIP = "1"
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
    ./build_release.sh -m pcie -t rfnm_dfe -f LA9310_DFE_APP=ON -f LA9310_TURN_ON_COMMAND_LINE=ON
    cp release/la9310.bin la9310_dfe.bin
    ./clean.sh
    ./build_release.sh -m pcie -t rfnm -b Release
    mv la9310_dfe.bin release
elif [ imx8mp-seeve ]; then
    ./build_release.sh -m pcie -t rfnm_dfe -f LA9310_DFE_APP=ON -f LA9310_TURN_ON_COMMAND_LINE=ON
    cp release/la9310.bin la9310_dfe.bin
    ./clean.sh
    ./build_release.sh -m pcie -t seeve -b Release
    mv la9310_dfe.bin release
else
    ./build_release.sh -m pcie -t nlm -b Release
fi

}

FILES:${PN}= "/lib"

do_install () {
        install -d ${D}${nonarch_base_libdir}/firmware
        install -m0755 ${S}/Demo/CORTEX_M4_NXP_LA9310_GCC/release/*.bin ${D}${nonarch_base_libdir}/firmware
        install -m0755 ${S}/firmware/*.eld ${D}${nonarch_base_libdir}/firmware
}

