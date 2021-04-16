# update kernel configuration
DELTA_KERNEL_DEFCONFIG_qoriq += "linux-baremetal.config linux-rt.config"
DELTA_KERNEL_DEFCONFIG_ls1028ardb += "ls1028a_linux-baremetal.config"
DELTA_KERNEL_DEFCONFIG_lx2160ardb += "lx2160a_linux-baremetal.config"

# update baremetal dts
KERNEL_DEVICETREE_ls1043ardb += "freescale/fsl-ls1043a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_ls1046ardb += "freescale/fsl-ls1046a-rdb-sdk-bm.dtb"
KERNEL_DEVICETREE_ls1028ardb += "freescale/fsl-ls1028a-rdb-sdk-bm.dtb"