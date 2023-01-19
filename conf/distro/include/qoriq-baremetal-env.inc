# update kernel configuration
DELTA_KERNEL_DEFCONFIG:append:ls1043ardb = " linux-baremetal-ls104xa.config"
DELTA_KERNEL_DEFCONFIG:append:ls1046ardb = " linux-baremetal-ls104xa.config"
DELTA_KERNEL_DEFCONFIG:append:ls1028ardb = " linux-baremetal-ls1028a.config"
DELTA_KERNEL_DEFCONFIG:append:lx2160a    = " linux-baremetal-lx2160a.config"
DELTA_KERNEL_DEFCONFIG:append:ls1021aiot = " linux-baremetal-ls1021a.config"

# update baremetal dts
KERNEL_DEVICETREE:append:ls1021aiot = " ls1021a-iot-bm.dtb"
KERNEL_DEVICETREE:append:ls1043ardb = " freescale/fsl-ls1043a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE:append:ls1046ardb = " freescale/fsl-ls1046a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE:append:ls1028ardb = " freescale/fsl-ls1028a-rdb-sdk-bm.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG:ls1021aiot ?= "ls1021aiot_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:ls1043ardb ?= "ls1043ardb_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:ls1046ardb ?= "ls1046ardb_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:ls1028ardb ?= "ls1028ardb_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:lx2160ardb ?= "lx2160ardb_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:lx2160ardb-rev2 ?= "lx2160ardb_baremetal_master_defconfig"

UBOOT_BOOT_BINARY ?= "u-boot-dtb.bin"
UBOOT_BOOT_BINARY:ls1021aiot ?= "u-boot-with-spl-pbl.bin"

UBOOT_CONFIG = "tfa"
UBOOT_CONFIG[tfa] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG},,${UBOOT_BOOT_BINARY}"

uboot_boot = "${DEPLOY_DIR_IMAGE}/u-boot.bin-tfa"
uboot = "${DEPLOY_DIR_IMAGE}/u-boot.bin-tfa"

# sai
RCWSD:ls1028ardb = "${@bb.utils.contains('DISTRO_FEATURES', 'sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600_sai', \
                    'R_SQPP_0x85bb/rcw_1500_gpu600', d)}"
RCWSD:ls1046ardb = "RR_FFSSPPPH_1133_5559/rcw_1800_gpio2_sdboot"
