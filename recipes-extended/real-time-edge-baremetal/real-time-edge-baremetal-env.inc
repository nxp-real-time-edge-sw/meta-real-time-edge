# uboot env
# layerscape uboot
UBOOT_BAREMETAL_DEFCONFIG:ls1043ardb ?= "ls1043ardb_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:ls1046ardb ?= "ls1046ardb_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:lx2160ardb ?= "lx2160ardb_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:lx2160ardb-rev2 ?= "lx2160ardb_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:ls1021aiot ?= "ls1021aiot_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:ls1028ardb ?= "${@bb.utils.contains('DISTRO_FEATURES', 'sai', \
                    'ls1028ardb_baremetal_slave_sai_defconfig', \
                    'ls1028ardb_baremetal_slave_defconfig', d)}"

# imx uboot
UBOOT_BAREMETAL_DEFCONFIG:mx8mp-nxp-bsp ?= "imx8mp_evk_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:mx8mm-nxp-bsp ?= "imx8mm_evk_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:imx93evk ?= "imx93_11x11_evk_baremetal_slave_defconfig"
UBOOT_BAREMETAL_DEFCONFIG:imx93-9x9-lpddr4-qsb ?= "imx93_9x9_qsb_baremetal_slave_defconfig"
