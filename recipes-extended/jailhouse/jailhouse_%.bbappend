FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

REAL_TIME_EDGE_JAILHOUSE_SRC ?= "git://github.com/nxp-imx/imx-jailhouse.git;protocol=https"

SRC_URI = "${REAL_TIME_EDGE_JAILHOUSE_SRC};branch=${SRCBRANCH} \
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
           file://scripts/linux-demo-ls1028ardb-enetc.sh \
           file://scripts/uart-demo-imx8mp.sh \
           file://scripts/gic-demo-imx8mp.sh \
           file://scripts/ivshmem-demo-imx8mp.sh \
           file://scripts/linux-demo-imx8mp.sh \
           file://rootfs.cpio.gz \
           file://0009-configs-ls1043a-rdb-add-fman-ucode-memory-for-root-c.patch \
           file://0010-configs-ls1043ardb-Add-gpio1-in-non-root-config-and-.patch \
           file://0011-config-ls1046ardb-modify-memory-range-align-with-ls1.patch \
           file://0012-configs-ls1046ardb-add-memory-for-root-cell-fix-band.patch \
           file://0013-configs-ls1046ardb-Add-fman-ucode-dtsi-file.patch \
           file://0014-configs-ls1046ardb-Add-dpaa-support-for-non-root-lin.patch \
           file://0015-configs-ls1046ardb-Add-gpio1-in-config-and-dts-file.patch \
           file://0018-update-ls1028a-rdb-config-and-dts-for-openil.patch \
           file://0027-ls1028ardb-add-gpio3-for-linux-demo.patch \
           file://0028-ls1028ardb-enetc-support-in-jailhouse.patch \
           file://0029-add-back-virtual-pci-support-in-root-cell.patch \
           file://0030-configs-arm64-dts-Change-the-reserved-address-for-LP.patch \
           file://0031-configs-arm64-dts-Correct-the-StreamID-of-ls1028a-EN.patch \
           file://0032-configs-arm64-Add-GIC-distributor-and-re-distributor.patch \
           file://0033-configs-arm64-ls1028a-add-LPI-tables-memory-region.patch \
           file://0034-ivshmem-demo-change-the-id-to-2-for-qoriq-platforms.patch \
           file://0035-configs-arm64-add-ivshmem-demo-support-for-8mp.patch \
           file://0036-ls1043ardb-inmate-demo-assigned-write-permission-to-.patch \
           file://0037-ls1028ardb-remove-gic-distributor-region-from-linux-.patch \
"

DEPENDS += " \
    python3-zipp \
"

do_install:append() {
    install -d ${D}${JH_DATADIR}/scripts
    install ${S}/../scripts/*.sh ${D}${JH_DATADIR}/scripts

    install -d ${D}${INMATES_DIR}/rootfs
    install ${S}/../rootfs.cpio ${D}${INMATES_DIR}/rootfs/

    install -d ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1043a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1046a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-ls1028a*.dtb ${D}${INMATES_DIR}/dtb
    install ${B}/configs/${JH_ARCH}/dts/inmate-imx8mp-evk.dtb ${D}${INMATES_DIR}/dtb

    install -d ${D}${INMATES_DIR}/kernel

    install ${B}/tools/jailhouse ${D}${JH_DATADIR}/tools
    install ${B}/tools/demos/ivshmem-demo ${D}${JH_DATADIR}/tools
}

RDEPENDS:${PN} += " \
    python3-zipp \
    python3-ctypes \
    python3-fcntl \
"

RDEPENDS:pyjailhouse += " \
    python3-zipp \
"

COMPATIBLE_MACHINE = "(qoriq|mx8-nxp-bsp|mx93-nxp-bsp)"
