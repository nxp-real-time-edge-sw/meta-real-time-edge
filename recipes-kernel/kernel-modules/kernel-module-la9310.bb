DESCRIPTION = "Kernel module for LA9310 PCIE driver."
LICENSE = "GPL-2.0-or-later"

require ../include/la93xx-repo.inc

LIC_FILES_CHKSUM = "file://license/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://license/bsd-3-clause.txt;md5=0f00d99239d922ffd13cabef83b33444"

inherit module

SRCREV_FORMAT = "la93xx-sdk"

SRC_URI = "${SRC_LA9310_HOST_URI} \
        ${SRC_LA9310_FRTOS_URI};destsuffix=la93xx_freertos \
        ${SRC_LA9310_FW_URI};destsuffix=git/firmware \
"

S = "${WORKDIR}/git"

MAKE_TARGETS = "all"

EXTRA_OEMAKE = " \
  CROSS_COMPILE='${TARGET_PREFIX} SYSROOT=${STAGING_DIR_TARGET}' \
  KERNEL_DIR='${STAGING_KERNEL_BUILDDIR}' \
  ARCH='arm64' \
  LA9310_COMMON_HEADERS='${WORKDIR}/la93xx_freertos/common_headers' \
"
EXTRA_OEMAKE:append:imx8mp-rfnm = " IMX_RFNM='1' "
PACKAGE_ARCH = "${MACHINE_ARCH}"

do_configure () {
  cd ${S}
  sed -i 's/DIRS ?= lib app kernel_driver firmware scripts/DIRS ?= kernel_driver/g' Makefile
}


do_install() {
        install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
        install -m 0755 ${S}/kernel_driver/la9310shiva/la9310shiva.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
	install -m 0755 ${S}/kernel_driver/la9310demo/la9310demo.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
if [ imx8mp-rfnm ]; then
	install -m 0755 ${S}/kernel_driver/la9310rfnm/la9310rfnm.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
	install -m 0755 ${S}/kernel_driver/la9310rfnm/rfnm_usb_function.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
	install -m 0755 ${S}/kernel_driver/la9310rfnm/rfnm_usb.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
fi
}
