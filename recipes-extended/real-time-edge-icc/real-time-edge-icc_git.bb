SUMMARY = "real-time-edge ICC"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b8b124def019f141a27e002e0a4333cc"

SRC_URI = "git://github.com/real-time-edge-sw/real-time-edge-icc.git;protocol=https;nobranch=1"
SRCREV = "552f495562071906639918938bab8b4766da7d4d"
PV = "1.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S = "${WORKDIR}/git"


ICC_GIC_OFFSET_ALIGN = ""
ICC_GIC_OFFSET_ALIGN_mx8m = "y"
ICC_GIC_OFFSET_ALIGN_ls1028a = "y"
ICC_GIC_OFFSET_ALIGN_ls1043a = "y"
ICC_GIC_OFFSET_ALIGN_ls1046a = "y"
ICC_GIC_OFFSET_ALIGN_lx2160a = "y"

ICC_GIC_TYPE = ""
ICC_GIC_TYPE_mx8m = "CONFIG_ICC_IMX8M"
ICC_GIC_TYPE_lx2160a = "CONFIG_ICC_LX2160A"

ICC_MAX_CPUS = "2"
ICC_MAX_CPUS_mx8m = "4"
ICC_MAX_CPUS_ls1043a = "4"
ICC_MAX_CPUS_ls1046a = "4"
ICC_MAX_CPUS_lx2160a = "16"


do_configure() {

    if [ "${ICC_GIC_OFFSET_ALIGN}" = "y" ]; then
        echo "#define CONFIG_ICC_GIC_OFFSET_ALIGN" > ${S}/icc_configure.h
    fi

    if [ "${ICC_GIC_TYPE}" != "" ]; then
        echo "#define ${ICC_GIC_TYPE}" >> ${S}/icc_configure.h
    fi

    echo "#define CONFIG_ICC_MAX_CPUS ${ICC_MAX_CPUS}" >> ${S}/icc_configure.h
}

do_install () {
    install -d ${D}/${bindir}
    install -p ${S}/icc  ${D}/${bindir}
}
