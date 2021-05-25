# RT Edge base conf

DISTRO = "nxp-rt-edge"
DISTROOVERRIDES = "rt-edge"
DISTRO_NAME = "NXP RT Edge Distro"
DISTRO_VERSION = "1.12"

DISTRO_FEATURES_remove_mx8 = "jailhouse "
DISTRO_FEATURES_append_ls1043ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1046ardb = " jailhouse-rt-edge"
DISTRO_FEATURES_append_ls1028ardb = " jailhouse-rt-edge libbee libblep libnfc-nci \
				      wayland-protocols weston imx-gpu-viv libdrm kmscube \
				      sysrepo-tsn tsn-scripts wayland"
DISTRO_FEATURES_append_mx8mp = " jailhouse-rt-edge sysrepo-tsn"
DISTRO_FEATURES_append_mx8m = " wayland-protocols weston imx-gpu-viv libdrm kmscube"
DISTRO_FEATURES_append_ls1028atsn = " sysrepo-tsn"
DISTRO_FEATURES_append_ls1021atsn = " sysrepo-tsn nodejs-lbt feedgnuplot tsn-scripts prl"
DISTRO_FEATURES_append_ls1021aiot = " canfestival"

#For ls1028ardb
DISTRO_FEATURES_remove_ls1028ardb = " x11"
PREFERRED_VERSION_weston_ls1028ardb = "9.0.0.imx"
PREFERRED_VERSION_wayland-protocols_ls1028ardb = "1.20.imx"
PREFERRED_VERSION_libdrm_ls1028ardb = "2.4.102.imx"
PREFERRED_PROVIDER_virtual/libgl_ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1_ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2_ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/egl_ls1028ardb  = "imx-gpu-viv"
LICENSE_FLAGS_WHITELIST_append_ls1028ardb = " commercial"

DELTA_KERNEL_DEFCONFIG_append = " \
    linux-rt.config \
"
DELTA_KERNEL_DEFCONFIG_append_mx8mm = " linux-fec.config"
DELTA_KERNEL_DEFCONFIG_append_mx8m = " linux-imx8.config"

SYSREPO_TSN_ls1028ardb = "sysrepo-tsn-tc"
SYSREPO_TSN_ls1028atsn = "sysrepo-tsn-tc"
SYSREPO_TSN_ls1021atsn = "sysrepo-tsn-tc"
SYSREPO_TSN_mx8mp = "sysrepo-tsn-tc"
PACKAGECONFIG_append_pn-sysrepo-tsn = "${SYSREPO_TSN}"

# dtb for jailhouse
KERNEL_DEVICETREE_append_ls1043ardb= " freescale/fsl-ls1043a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE_append_ls1043ardb= " freescale/fsl-ls1043a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE_append_ls1046ardb= " freescale/fsl-ls1046a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE_append_ls1046ardb= " freescale/fsl-ls1046a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE_append_ls1028ardb= " freescale/fsl-ls1028a-rdb-jailhouse.dtb"
KERNEL_DEVICETREE_append_imx8mpevk = " freescale/imx8mp-evk-root.dtb"
