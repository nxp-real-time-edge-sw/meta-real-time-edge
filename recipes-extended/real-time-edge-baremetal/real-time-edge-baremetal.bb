# Copyright 2021-2025 NXP

require real-time-edge-baremetal-env.inc
inherit fsl-u-boot-localversion deploy

SUMMARY = "U-boot-baremetal provided by NXP"
LICENSE = "GPL-2.0-only & BSD-3-Clause & BSD-2-Clause & LGPL-2.0-only & LGPL-2.1-only"
LIC_FILES_CHKSUM = " \
    file://Licenses/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
    file://Licenses/bsd-2-clause.txt;md5=6a31f076f5773aabd8ff86191ad6fdd5 \
    file://Licenses/bsd-3-clause.txt;md5=4a1190eac56a9db675d58ebe86eaf50c \
    file://Licenses/lgpl-2.0.txt;md5=4cf66a4984120007c9881cc871cf49db \
    file://Licenses/lgpl-2.1.txt;md5=4fbd65380cdd255951079008b364516c \
"
UBOOT_BAREMETAL_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-uboot.git;protocol=https"
UBOOT_BAREMETAL_BRANCH ?= "baremetal-uboot_v2025.04-3.2.0"
UBOOT_BAREMETAL_SRCREV ?= "c7feda2f50823193bf25e608013a854f592e2448"

SRC_URI = "${UBOOT_BAREMETAL_SRC};branch=${UBOOT_BAREMETAL_BRANCH}"

SRCREV = "${UBOOT_BAREMETAL_SRCREV}"

PV = "2025.04+git${SRCPV}"

PROVIDES = "real-time-edge-baremetal"

S = "${WORKDIR}/git"
B = "${WORKDIR}/build"

FILES:${PN} = "/boot"

UBOOT_BAREMETAL_DEFCONFIG ?= ""
DELTA_UBOOT_BAREMETAL_DEFCONFIG ?= ""
UBOOT_BAREMETAL_MAKE_TARGET ?= "all"
UBOOT_BAREMETAL_BINARY ?= "u-boot.bin"
UBOOT_BAREMETAL_RENAME ?= "bm-${UBOOT_BAREMETAL_BINARY}"
PACKAGE_ARCH = "${MACHINE_ARCH}"

DEPENDS = "kern-tools-native swig-native"
DEPENDS += "dtc-native bison-native bc-native"

EXTRA_OEMAKE = 'CROSS_COMPILE=${TARGET_PREFIX} CC="${TARGET_PREFIX}gcc ${TOOLCHAIN_OPTIONS}" V=1'
EXTRA_OEMAKE += 'HOSTCC="${BUILD_CC} ${BUILD_CFLAGS} ${BUILD_LDFLAGS}"'
EXTRA_OEMAKE += 'STAGING_INCDIR=${STAGING_INCDIR_NATIVE} STAGING_LIBDIR=${STAGING_LIBDIR_NATIVE}'

do_configure () {
    if [ -n "${UBOOT_BAREMETAL_DEFCONFIG}" ]; then
        cp ${S}/configs/${UBOOT_BAREMETAL_DEFCONFIG} ${B}/.config
        for deltacfg in ${DELTA_UBOOT_BAREMETAL_DEFCONFIG}; do
            ${S}/scripts/kconfig/merge_config.sh -m .config ${deltacfg}
        done
        cp ${B}/.config ${S}/configs/${UBOOT_BAREMETAL_DEFCONFIG}

        oe_runmake -C ${S} O=${B} ${UBOOT_BAREMETAL_DEFCONFIG}
    fi
    DEVTOOL_DISABLE_MENUCONFIG=true
}

do_compile () {
    unset LDFLAGS
    unset CFLAGS
    unset CPPFLAGS

    if [ -n "${UBOOT_BAREMETAL_DEFCONFIG}" ]; then
        oe_runmake -C ${S} O=${B} ${UBOOT_BAREMETAL_MAKE_TARGET}
        cp ${B}/${UBOOT_BAREMETAL_BINARY} ${B}/${UBOOT_BAREMETAL_RENAME}
    fi
}

do_install () {
    if [ -n "${UBOOT_BAREMETAL_DEFCONFIG}" ]; then
        install -D -m 644 ${B}/${UBOOT_BAREMETAL_RENAME} ${D}/boot/${UBOOT_BAREMETAL_RENAME}
    fi
}

do_deploy () {
    if [ -n "${UBOOT_BAREMETAL_DEFCONFIG}" ]; then
        install -D -m 644 ${B}/${UBOOT_BAREMETAL_RENAME} ${DEPLOYDIR}/${UBOOT_BAREMETAL_RENAME}
    fi
}
addtask deploy after do_install

COMPATIBLE_MACHINE = "qoriq|imx"
