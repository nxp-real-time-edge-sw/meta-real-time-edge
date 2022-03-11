# update kernel configuration
DELTA_KERNEL_DEFCONFIG:append:mx8m = " linux-baremetal.config"

# update baremetal dts
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-baremetal.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG ?= ""

UBOOT_BOOT_BAREMETAL_DEFCONFIG:imx8mp-lpddr4-evk ?= "imx8mp_evk_baremetal_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:imx8mm-lpddr4-evk ?= "imx8mm_evk_baremetal_defconfig"

UBOOT_CONFIG = "tfa"
UBOOT_CONFIG[tfa] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG}"
