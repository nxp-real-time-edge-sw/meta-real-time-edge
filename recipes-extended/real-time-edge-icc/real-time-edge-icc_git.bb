SUMMARY = "real-time-edge ICC"
LICENSE = "GPL-2.0-only"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b8b124def019f141a27e002e0a4333cc"

SRC_URI = "git://github.com/nxp-real-time-edge-sw/real-time-edge-icc.git;protocol=https;nobranch=1"
SRCREV = "972d26f976a75193414ce9076d577306d97cd98d"
PV = "1.1"

PACKAGE_ARCH = "${MACHINE_ARCH}"

S = "${WORKDIR}/git"


ICC_GIC_OFFSET_ALIGN = ""
ICC_GIC_OFFSET_ALIGN:mx8m-nxp-bsp = "y"
ICC_GIC_OFFSET_ALIGN:ls1028a = "y"
ICC_GIC_OFFSET_ALIGN:ls1043a = "y"
ICC_GIC_OFFSET_ALIGN:ls1046a = "y"
ICC_GIC_OFFSET_ALIGN:lx2160a = "y"

ICC_GIC_TYPE = ""
ICC_GIC_TYPE:mx8m-nxp-bsp = "CONFIG_ICC_IMX8M"
ICC_GIC_TYPE:lx2160a = "CONFIG_ICC_LX2160A"

ICC_MAX_CPUS = "2"
ICC_MAX_CPUS:mx8m-nxp-bsp = "4"
ICC_MAX_CPUS:ls1043a = "4"
ICC_MAX_CPUS:ls1046a = "4"
ICC_MAX_CPUS:lx2160a = "16"


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
