DESCRIPTION = "Precision Time Protocol (PTP) according to IEEE standard 1588 for Linux"

LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263"


SRC_URI = "git://bitbucket.sw.nxp.com/dnind/linux-ptp.git;protocol=ssh;nobranch=1"
SRCREV = "76e65a818a82e8ea4343fc088ef239a64e4996da"

S = "${WORKDIR}/git"

DEPENDS = "virtual/kernel"

export KBUILD_OUTPUT="${RECIPE_SYSROOT}"
export PKG_CONFIG_SYSROOT_DIR="${STAGING_KERNEL_BUILDDIR}"

EXTRA_OEMAKE = " \
    CC="${CC}" \
    CROSS_COMPILE="${TARGET_PREFIX}" \
    ARCH="${TARGET_ARCH}" \
    EXTRA_CFLAGS="${CFLAGS}" \
"

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/ptp4l  ${D}/${bindir}
    install -p ${S}/pmc  ${D}/${bindir}
    install -p ${S}/phc2sys  ${D}/${bindir}
    install -p ${S}/hwstamp_ctl  ${D}/${bindir}

    install -d ${D}/etc/ptp4l_cfg
    install ${S}/configs/* ${D}/etc/ptp4l_cfg
}
