DESCRIPTION = "user-space application for LA9310 PCI driver."
LICENSE = "GPL-2.0-or-later"

require ../include/la93xx-repo.inc

LIC_FILES_CHKSUM = "file://license/gpl-2.0.txt;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://license/bsd-3-clause.txt;md5=0f00d99239d922ffd13cabef83b33444"

SRCREV_FORMAT = "la93xx-sdk"

SRC_URI = "${SRC_LA9310_HOST_URI} \
        ${SRC_LA9310_FRTOS_URI};destsuffix=la93xx_freertos \
"

S = "${WORKDIR}/git"

MAKE_TARGETS = "all"

EXTRA_OEMAKE = '\
    CROSS_COMPILE=${TARGET_PREFIX} \
    KERNEL_DIR=${STAGING_KERNEL_BUILDDIR} \
    LA9310_COMMON_HEADERS=${WORKDIR}/la93xx_freertos/common_headers \
    ARCH=${ARCH} \
    CFLAGS="${CFLAGS} -O2" \
    CC="${CC}" \
    AR="${AR}" \
    LD="${LD}" \
    DESTDIR="${D}" \
'
PARALLEL_MAKE="-j 1"
FILES:${PN}= "/usr/lib/* /usr/bin/* ${ROOT_HOME}/* /lib/firmware/* "

do_configure () {
	cd ${S}
	sed -i 's/DIRS ?= lib app kernel_driver firmware scripts/DIRS ?= lib app scripts/g' Makefile
}


do_compile() {
	unset LDFLAGS CFLAGS CPPFLAGS BUILD_LDFLAGS
	oe_runmake -C ${S}
	oe_runmake install
}

do_install() {
	install -d ${D}${libdir}
	install -d ${D}/usr/bin
	install -m 0755 ${S}/install/usr/lib/libla9310wdog.so ${D}${libdir}
        install -m 0755 ${S}/install/usr/lib/libla9310tti.so ${D}${libdir}
	install -m 0755 ${S}/install/usr/bin/* ${D}/usr/bin/
	install -d ${D}/${ROOT_HOME}
	install -d ${D}/${ROOT_HOME}/host_utils
	install -d ${D}${nonarch_base_libdir}/firmware
	install -m 0755 ${S}/install/etc/*.sh ${D}/${ROOT_HOME}/
	install -m 0755 ${S}/install/etc/rfnm_* ${D}/${ROOT_HOME}/
	install -m 0755 ${S}/app/host_utils/*.py ${D}/${ROOT_HOME}/host_utils
	install -m 0755 ${S}/app/host_utils/*.sh ${D}/${ROOT_HOME}/host_utils
	install -m 0755 ${S}/app/host_utils/*.bin ${D}/${ROOT_HOME}/host_utils
	install -m 0755 ${S}/app/host_utils/fw_iqplayer/*.eld ${D}${nonarch_base_libdir}/firmware
	install -m 0755 ${S}/install/usr/lib/libecspi.a "${TMPDIR}/work-shared/imx8mp-rfnm/"
}

INSANE_SKIP:${PN} = "ldflags"
CLEANBROKEN = "1"
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_SYSROOT_STRIP = "1"
SOLIBS = ".so"
FILES_SOLIBSDEV = ""
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
do_package_qa[noexec] = "1"
