DESCRIPTION = "Kernel module for accessing any MDIO device."
LICENSE = "GPL-2.0-or-later"

LIC_FILES_CHKSUM = "file://license/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://license/bsd-3-clause.txt;md5=0f00d99239d922ffd13cabef83b33444"

inherit module

SRC_URI = "git://github.com/nxp-qoriq/la93xx_host_sw;protocol=https;nobranch=1;name=host_repo \
	   git://github.com/nxp-qoriq/la93xx_freertos;protocol=https;nobranch=1;name=freertos_repo;destsuffix=la93xx_freertos \
	   git://github.com/nxp-qoriq/la93xx_firmware;protocol=https;nobranch=1;name=firmware_repo;destsuffix=git/firmware \
"


SRCREV_host_repo = "9ee1861f2134c496e88699e41211e9501b4a49da"
SRCREV_freertos_repo = "92cb58f9e90e29ada28feab64e8c54d95067504b"
SRCREV_firmware_repo = "c298c417f689f0f6709a94d1c75156b1ed2934f5"

S = "${WORKDIR}/git"

MAKE_TARGETS = "all"

EXTRA_OEMAKE = " \
  CROSS_COMPILE='${TARGET_PREFIX} SYSROOT=${STAGING_DIR_TARGET}' \
  KERNEL_DIR='${STAGING_KERNEL_BUILDDIR}' \
  ARCH='arm64' \
  LA9310_COMMON_HEADERS='${WORKDIR}/la93xx_freertos/common_headers' \
"


do_configure () {
  cd ${S}
  sed -i 's/DIRS ?= lib app kernel_driver firmware scripts/DIRS ?= kernel_driver/g' Makefile
}


do_install() {
        install -d ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
        install -m 0755 ${S}/kernel_driver/la9310shiva/la9310shiva.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
	install -m 0755 ${S}/kernel_driver/la9310demo/la9310demo.ko ${D}${nonarch_base_libdir}/modules/${KERNEL_VERSION}/extra
}
