# update kernel configuration
DELTA_KERNEL_DEFCONFIG_append_mx8m = " linux-baremetal.config"

# update baremetal dts
KERNEL_DEVICETREE_append_imx8mpevk = " freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE_append_imx8mmevk = " freescale/imx8mm-evk-baremetal.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG ?= ""

UBOOT_BOOT_BAREMETAL_DEFCONFIG_imx8mpevk ?= "imx8mp_evk_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_imx8mmevk ?= "imx8mm_evk_baremetal_defconfig"

UBOOT_CONFIG = "tfa"
UBOOT_CONFIG[tfa] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG}"
