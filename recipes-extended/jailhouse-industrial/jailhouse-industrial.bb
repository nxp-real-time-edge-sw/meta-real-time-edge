SUMMARY = "Jailhouse, OpenIL fork"
HOMEPAGE = "https://github.com/siemens/jailhouse"
SECTION = "jailhouse-industrial"
LICENSE = "GPL-2.0"

LIC_FILES_CHKSUM = "file://COPYING;md5=9fa7f895f96bde2d47fd5b7d95b6ba4d \
                 file://tools/root-cell-config.c.tmpl;beginline=6;endline=33;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://include/jailhouse/hypercall.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://include/jailhouse/cell-config.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://include/arch/arm/asm/jailhouse_hypercall.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://include/arch/arm64/asm/jailhouse_hypercall.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://include/arch/x86/asm/jailhouse_hypercall.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
                 file://driver/jailhouse.h;beginline=9;endline=36;md5=2825581c1666c44a17955dc574cfbfb3 \
"

SRCBRANCH = "master"
QORIQ_JAILHOUSE_SRC ?= "git://bitbucket.sw.nxp.com/dnind/industrial-jailhouse.git;protocol=ssh"

SRC_URI = "${QORIQ_JAILHOUSE_SRC};branch=${SRCBRANCH} \
           file://0001-tools-scripts-update-shebang-to-python3.patch \
           file://scripts/init_jailhouse_env.sh \
           file://scripts/uart-demo-ls1043ardb.sh \
           file://scripts/gic-demo-ls1043ardb.sh \
           file://scripts/ivshmem-demo-ls1043ardb.sh \
           file://scripts/linux-demo-ls1043ardb.sh \
           file://scripts/linux-demo-ls1043ardb-dpaa.sh \
           file://scripts/uart-demo-ls1046ardb.sh \
           file://scripts/gic-demo-ls1046ardb.sh \
           file://scripts/ivshmem-demo-ls1046ardb.sh \
           file://scripts/linux-demo-ls1046ardb.sh \
           file://scripts/linux-demo-ls1046ardb-dpaa.sh \
           file://scripts/uart-demo-ls1028ardb.sh \
           file://scripts/gic-demo-ls1028ardb.sh \
           file://scripts/ivshmem-demo-ls1028ardb.sh \
           file://scripts/linux-demo-ls1028ardb.sh \
           file://scripts/uart-demo-imx8mp.sh \
           file://scripts/gic-demo-imx8mp.sh \
           file://scripts/ivshmem-demo-imx8mp.sh \
           file://scripts/linux-demo-imx8mp.sh \
"

SRCREV = "6de9e2c1bc2c8a6b47a1d2dcc9972aeae5a5a198"

DEPENDS = " \
    make-native \
    python3-mako-native \
    python3-mako \
    python3-zipp \
    dtc-native \
"

inherit module python3native bash-completion deploy setuptools3

S = "${WORKDIR}/git"
B = "${S}"

JH_ARCH = "arm64"
JH_DATADIR ?= "${datadir}/jailhouse"
CELL_DIR ?= "${JH_DATADIR}/cells"
CELLCONF_DIR ?= "${JH_DATADIR}/configs"
INMATES_DIR ?= "${JH_DATADIR}/inmates"
SCRIPT_DIR ?= "${JH_DATADIR}/scripts"

JH_CONFIG ?= "${S}/ci/jailhouse-config-x86.h"
JH_CONFIG_x86 ?= "${S}/ci/jailhouse-config-x86.h"
JH_CONFIG_x86-64 ?= "${S}/ci/jailhouse-config-x86.h"
JH_CONFIG_arm ?= "${S}/ci/jailhouse-config-banana-pi.h"

do_configure() {
   if [ -d ${STAGING_DIR_HOST}/${CELLCONF_DIR} ];
   then
      cp "${STAGING_DIR_HOST}/${CELLCONF_DIR}/"*.c ${S}/configs/${ARCH}/
   fi
}

USER_SPACE_CFLAGS = '${CFLAGS} -DLIBEXECDIR=\\\"${libexecdir}\\\" \
		  -DJAILHOUSE_VERSION=\\\"$JAILHOUSE_VERSION\\\" \
		  -Wall -Wextra -Wmissing-declarations -Wmissing-prototypes -Werror \
		  -I../driver'

TOOLS_SRC_DIR = "${S}/tools"
TOOLS_OBJ_DIR = "${S}/tools"

do_compile() {
    unset LDFLAGS
    oe_runmake V=1 CC="${CC}" \
        ARCH=${JH_ARCH} CROSS_COMPILE=${TARGET_PREFIX} \
        KDIR=${STAGING_KERNEL_BUILDDIR}

    cd ${TOOLS_SRC_DIR}
    export JAILHOUSE_VERSION=$(cat ../VERSION)
    oe_runmake V=1 \
        CFLAGS="${USER_SPACE_CFLAGS}" \
        src=${TOOLS_SRC_DIR} obj=${TOOLS_OBJ_DIR} \
        ${TOOLS_OBJ_DIR}/jailhouse-config-collect ${TOOLS_OBJ_DIR}/jailhouse
}

do_install() {
    oe_runmake \
        PYTHON=python3 \
        V=1 \
        LDFLAGS="" \
        CC="${CC}" \
        ARCH=${JH_ARCH} \
        CROSS_COMPILE=${TARGET_PREFIX} \
        KDIR=${STAGING_KERNEL_BUILDDIR} \
        DESTDIR=${D} install

    install -d ${D}${CELL_DIR}
    install ${B}/configs/${JH_ARCH}/*.cell ${D}${CELL_DIR}/

    install -d ${D}${SCRIPT_DIR}
    install ${S}/../scripts/*.sh ${D}${SCRIPT_DIR}/

    install -d ${D}${INMATES_DIR}/tools/${JH_ARCH}
    install ${B}/inmates/demos/${JH_ARCH}/*.bin ${D}${INMATES_DIR}

    install -d ${D}${INMATES_DIR}/rootfs
    install ${B}/inmates/rootfs/rootfs.cpio.gz ${D}${INMATES_DIR}/rootfs

    install -d ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1043a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1046a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1046a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-imx8mp-evk.dtb ${D}${INMATES_DIR}/dtb

    install -d ${D}${INMATES_DIR}/kernel

    install -d ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-cell-linux ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-cell-stats ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-config-collect ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-config-create ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-gcov-extract ${D}${JH_DATADIR}/tools
    install ${B}/tools/jailhouse-hardware-check ${D}${JH_DATADIR}/tools
    install ${B}/tools/demos/ivshmem-demo ${D}${JH_DATADIR}/tools
    install ${B}/inmates/tools/${JH_ARCH}/linux-loader.bin ${D}${INMATES_DIR}/tools/${JH_ARCH}
    install -d ${D}${PYTHON_SITEPACKAGES_DIR}/pyjailhouse
    install -m 0644 ${S}/pyjailhouse/*.py ${D}${PYTHON_SITEPACKAGES_DIR}/pyjailhouse
}

PACKAGE_BEFORE_PN = "kernel-module-jailhouse pyjailhouse"

FILES_${PN} += "${nonarch_base_libdir}/firmware ${libexecdir} ${sbindir} ${JH_DATADIR}"
FILES_pyjailhouse = "{PYTHON_SITEPACKAGES_DIR}/pyjailhouse"

RDEPENDS_${PN} += " \
    python3-curses \
    python3-datetime \
    python3-zipp \
    python3-mmap \
    python3-ctypes \
    python3-fcntl \
"

RDEPENDS_pyjailhouse = " \
    python3-core \
    python3-ctypes \
    python3-zipp \
    python3-fcntl \
    python3-shell \
"

INSANE_SKIP_${PN} = "ldflags"

COMPATIBLE_MACHINE = "(qoriq|mx8)"
