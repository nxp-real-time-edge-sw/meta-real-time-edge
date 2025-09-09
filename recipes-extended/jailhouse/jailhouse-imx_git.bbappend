
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append:qoriq = "file://0001-Revert-driver-sysfs-Update-bin_attribute-version-che.patch"
SRCBRANCH = "lf-6.12.34_2.1.0"
SRCREV = "f64de0b8f61b45b42f197c8212b6c0fe887e5c19"

PV = "2023.03+git${SRCPV}"

ERROR_QA:remove = "buildpaths"
WARN_QA:append = " buildpaths"

INSANE_SKIP:${PN}:remove = "ldflags"
ERROR_QA:remove = "ldflags"
WARN_QA:append = " ldflags"

RDEPENDS:${PN} += " \
    pyjailhouse \
"

SRC_URI += " \
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
    file://scripts/uart-demo-imx93.sh \
    file://scripts/gic-demo-imx93.sh \
    file://scripts/ivshmem-demo-imx93.sh \
    file://scripts/linux-demo-imx93-9x9-qsb.sh \
    file://rootfs.cpio.gz \
    file://0001-configs-ls1043a-rdb-add-fman-ucode-memory-for-root-c.patch \
    file://0002-configs-ls1043ardb-Add-gpio1-in-non-root-config-and-.patch \
    file://0003-config-ls1046ardb-modify-memory-range-align-with-ls1.patch \
    file://0004-configs-ls1046ardb-add-memory-for-root-cell-fix-band.patch \
    file://0005-configs-ls1046ardb-Add-fman-ucode-dtsi-file.patch \
    file://0006-configs-ls1046ardb-Add-dpaa-support-for-non-root-lin.patch \
    file://0007-configs-ls1046ardb-Add-gpio1-in-config-and-dts-file.patch \
    file://0008-update-ls1028a-rdb-config-and-dts-for-openil.patch \
    file://0009-ls1028ardb-add-gpio3-for-linux-demo.patch \
    file://0010-ls1028ardb-enetc-support-in-jailhouse.patch \
    file://0011-add-back-virtual-pci-support-in-root-cell.patch \
    file://0012-configs-arm64-dts-Change-the-reserved-address-for-LP.patch \
    file://0013-configs-arm64-dts-Correct-the-StreamID-of-ls1028a-EN.patch \
    file://0014-configs-arm64-Add-GIC-distributor-and-re-distributor.patch \
    file://0015-configs-arm64-ls1028a-add-LPI-tables-memory-region.patch \
    file://0016-ivshmem-demo-change-dev_id-to-2-for-qoriq-platforms.patch \
    file://0017-ls1043ardb-inmate-demo-assigned-write-permission-to-.patch \
    file://0018-ls1028ardb-remove-gic-distributor-region-from-linux-.patch \
    file://0019-configs-arm64-ls1028a-Update-the-address-used-for-GI.patch \
    file://0020-configs-arm64-fix-the-.architecture-for-ls1028ardb-e.patch \
    file://0021-configs-arm64-fix-the-.architecture-for-ls1046ardb-d.patch \
    file://0022-configs-ls1028ardb-add-SerDes-MMIO-region.patch \
"

DEPENDS += " \
    python3-zipp \
"

do_install:append() {
    install -d ${D}${JH_DATADIR}/scripts
    install ${UNPACKDIR}/scripts/*.sh ${D}${JH_DATADIR}/scripts

    install -d ${D}${INMATES_DIR}/rootfs
    install ${UNPACKDIR}/rootfs.cpio ${D}${INMATES_DIR}/rootfs/

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

COMPATIBLE_MACHINE:append = "|(qoriq)"
