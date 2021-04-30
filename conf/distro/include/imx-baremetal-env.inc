# update kernel configuration
DELTA_KERNEL_DEFCONFIG_append_imx8 = " linux-baremetal.config"

# update baremetal dts
KERNEL_DEVICETREE_append_mx8mp = " freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE_append_mx8mm = " freescale/imx8mm-evk-baremetal.dtb"
