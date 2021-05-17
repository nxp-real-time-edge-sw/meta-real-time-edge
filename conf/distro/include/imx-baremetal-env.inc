# update kernel configuration
DELTA_KERNEL_DEFCONFIG_append_imx8 = " linux-baremetal.config"

# update baremetal dts
KERNEL_DEVICETREE_append_mx8mp = " freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE_append_mx8mm = " freescale/imx8mm-evk-baremetal.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG ?= ""

UBOOT_BOOT_BAREMETAL_DEFCONFIG_imx8mpevk ?= "imx8mp_evk_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG_imx8mmevk ?= "imx8mm_evk_baremetal_defconfig"

UBOOT_CONFIG = "baremetal"
UBOOT_CONFIG[baremetal] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG}"