# update kernel configuration
DELTA_KERNEL_DEFCONFIG_qoriq += " linux-baremetal.config"
DELTA_KERNEL_DEFCONFIG_ls1028ardb += "linux-baremetal-ls1028a.config"
DELTA_KERNEL_DEFCONFIG_lx2160ardb += "linux-baremetal-lx2160a.config"
DELTA_KERNEL_DEFCONFIG_ls1021aiot += "linux-baremetal-ls1021a.config"

# update baremetal dts
KERNEL_DEVICETREE_ls1021aiot += "ls1021a-iot-bm.dtb"
KERNEL_DEVICETREE_ls1043ardb += "freescale/fsl-ls1043a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_ls1046ardb += "freescale/fsl-ls1046a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_ls1028ardb += "freescale/fsl-ls1028a-rdb-sdk-bm.dtb"
