# update kernel configuration
DELTA_KERNEL_DEFCONFIG_imx8 += "linux-baremetal.config linux-rt.config"

# update baremetal dts
KERNEL_DEVICETREE_mx8mp += "freescale/imx8mp-evk-baremetal.dtb"
KERNEL_DEVICETREE_mx8mm += "freescale/imx8mm-evk-baremetal.dtb"
