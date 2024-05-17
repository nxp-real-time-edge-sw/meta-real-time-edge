# Copyright 2024 NXP

require ../include/la93xx-repo.inc

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

# find the patch(es) in SRC_URI.
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRCREV_FORMAT = "la93xx-sdk"


SRC_URI = "\
    ${SRC_LA9310_FREERTOS_VARISCITE_URI};name=freertos_variscite_repo \
    ${SRC_LA9310_IMX8MP_M7_URI};name=imx8mp-m7_repo;destsuffix=git/boards/dart_mx8mp/driver_examples/gpt/imx8mp-m7 \
    "
SRC_URI += "${@bb.utils.contains('NXP_INTERNAL', '0', 'file://0001-rfnm-add-support-to-compile-m7.patch;patchdir=boards/dart_mx8mp/driver_examples/gpt/imx8mp-m7/', '', d)}"

inherit cmake

python () {
    d.delVar('CFLAGS')
    d.delVar('CXXFLAGS')
    d.delVar('LDFLAGS')
}


IMAGE_FSTYPES = "elf bin"

PACKAGES = "${PN}"

inherit deploy

do_configure[noexec] = "1"
do_package_qa[noexec] = "1"

INHIBIT_DEFAULT_DEPS = "1"
INHIBIT_SYSROOT_STRIP = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"

S = "${WORKDIR}/git"
IMX8MP-M7_SRC_DIR = "${S}/boards/dart_mx8mp/driver_examples/gpt/imx8mp-m7/"
DEPENDS = "gcc-arm-none-eabi-native"
ARMGCC_DIR ?= "${WORKDIR}/recipe-sysroot-native${libexecdir}/gcc-arm-none-eabi"


do_compile() {
    export ARMGCC_DIR="${ARMGCC_DIR}"
    echo "Start to compile la93xx with ${ARMGCC_DIR}"
    cd ${IMX8MP-M7_SRC_DIR}/armgcc
    chmod u+x *.sh
    ./clean.sh
    ./build_release.sh
}

FILES:${PN}= "/lib"

do_install () {
        install -d ${D}${nonarch_base_libdir}/firmware
        install -m0755 ${IMX8MP-M7_SRC_DIR}/armgcc/release/rfnm_m7_v0.elf ${D}${nonarch_base_libdir}/firmware
}

