# update kernel configuration
DELTA_KERNEL_DEFCONFIG:append:mx8m-nxp-bsp = " linux-baremetal.config"
DELTA_KERNEL_DEFCONFIG:append:mx93-nxp-bsp = " linux-baremetal-imx93.config"

# update baremetal dts
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-baremetal.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-baremetal.dtb"

# uboot env
UBOOT_BOOT_BAREMETAL_DEFCONFIG ?= ""

UBOOT_BOOT_BAREMETAL_DEFCONFIG:imx8mp-lpddr4-evk ?= "imx8mp_evk_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:imx8mm-lpddr4-evk ?= "imx8mm_evk_baremetal_master_defconfig"
UBOOT_BOOT_BAREMETAL_DEFCONFIG:imx93evk ?= "imx93_11x11_evk_baremetal_master_defconfig"

UBOOT_CONFIG = "tfa"
UBOOT_CONFIG[tfa] = "${UBOOT_BOOT_BAREMETAL_DEFCONFIG}"
