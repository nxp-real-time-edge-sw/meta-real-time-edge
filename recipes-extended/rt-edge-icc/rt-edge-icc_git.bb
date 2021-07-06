SUMMARY = "RT-Edge ICC"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/GPL-2.0;md5=801f80980d171dd6425610833a22dbe6"

SRC_URI = "git://github.com/rt-edge-sw/rt-edge-icc.git;protocol=https;nobranch=1"
SRCREV = "7438ebe3a7432d330d8b2ed9092488f7769f79b1"
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
