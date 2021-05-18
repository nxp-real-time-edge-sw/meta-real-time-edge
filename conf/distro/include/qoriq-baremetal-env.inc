# update kernel configuration
DELTA_KERNEL_DEFCONFIG_append_qoriq = " linux-baremetal.config"
DELTA_KERNEL_DEFCONFIG_append_ls1028ardb = " linux-baremetal-ls1028a.config"
DELTA_KERNEL_DEFCONFIG_append_lx2160ardb = " linux-baremetal-lx2160a.config"
DELTA_KERNEL_DEFCONFIG_append_ls1021aiot = " linux-baremetal-ls1021a.config"

# update baremetal dts
KERNEL_DEVICETREE_append_ls1021aiot = " ls1021a-iot-bm.dtb"
KERNEL_DEVICETREE_append_ls1043ardb = " freescale/fsl-ls1043a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_append_ls1046ardb = " freescale/fsl-ls1046a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_append_ls1028ardb = " freescale/fsl-ls1028a-rdb-sdk-bm.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG_ls1021aiot ?= "ls1021aiot_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_ls1043ardb ?= "ls1043ardb_tfa_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_ls1046ardb ?= "ls1046ardb_tfa_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_ls1028ardb ?= "ls1028ardb_tfa_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_lx2160ardb ?= "lx2160ardb_tfa_baremetal_defconfig"

UBOOT_BOOT_BINARY ?= "u-boot-dtb.bin"
UBOOT_BOOT_BINARY_ls1021aiot ?= "u-boot-with-spl-pbl.bin"

UBOOT_CONFIG = "baremetal"
UBOOT_CONFIG[baremetal] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG},,${UBOOT_BOOT_BINARY}"

uboot_boot = "${DEPLOY_DIR_IMAGE}/u-boot.bin-baremetal"
uboot = "${DEPLOY_DIR_IMAGE}/u-boot.bin-baremetal"