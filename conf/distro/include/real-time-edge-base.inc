# Real-time Edge base conf

DISTRO = "nxp-real-time-edge"
DISTROOVERRIDES = "real-time-edge"
DISTRO_NAME = "NXP Real-time Edge Distro"
DISTRO_VERSION = "2.2"

CONNECTIVITY_CHECK_URIS ?= "https://yoctoproject.org/connectivity.html"

DISTRO_FEATURES:append:ls1043ardb = " jailhouse"
DISTRO_FEATURES:append:ls1046ardb = " jailhouse"
DISTRO_FEATURES:append:ls1028ardb = " jailhouse real-time-edge-libbee \
				      real-time-edge-libblep libnfc-nci \
				      wayland-protocols weston imx-gpu-viv libdrm kmscube \
				      real-time-edge-sysrepo tsn-scripts wayland alsa"
DISTRO_FEATURES:append:mx8dxl-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:mx8mp-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:mx8m-nxp-bsp = " wayland-protocols weston imx-gpu-viv libdrm kmscube"
DISTRO_FEATURES:append:mx93-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:ls1021atsn = " real-time-edge-sysrepo feedgnuplot tsn-scripts real-time-edge-prl"
DISTRO_FEATURES:append:ls1021aiot = " canfestival"

# For ls1028ardb
DISTRO_FEATURES:remove:ls1028ardb = " x11"
PREFERRED_VERSION_weston:ls1028ardb = "10.0.1.imx"
PREFERRED_VERSION_wayland-protocols:ls1028ardb = "1.25.imx"
PREFERRED_VERSION_libdrm:ls1028ardb = "2.4.109.imx"
PREFERRED_PROVIDER_virtual/libgl:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/egl:ls1028ardb  = "imx-gpu-viv"
LICENSE_FLAGS_ACCEPTED:append:ls1028ardb = " commercial"
DISTROOVERRIDES:append:ls1028ardb = ":imxdrm"

DELTA_KERNEL_DEFCONFIG:append:mx8mm-nxp-bsp = " linux-fec.config"
DELTA_KERNEL_DEFCONFIG:append:mx8dxl-nxp-bsp = " linux-imx8.config"
DELTA_KERNEL_DEFCONFIG:append:mx8m-nxp-bsp = " linux-imx8.config"
DELTA_KERNEL_DEFCONFIG:append:mx93-nxp-bsp = " linux-imx8.config"
DELTA_KERNEL_DEFCONFIG:append:ls1043ardb = " linux-dpaa-ethercat.config"
DELTA_KERNEL_DEFCONFIG:append:ls1046ardb = " linux-dpaa-ethercat.config"

REAL_TIME_EDGE_SYSREPO:ls1028ardb = ""
REAL_TIME_EDGE_SYSREPO:ls1021atsn = "real-time-edge-sysrepo-tc"
REAL_TIME_EDGE_SYSREPO:mx8dxl-nxp-bsp = "real-time-edge-sysrepo-tc"
REAL_TIME_EDGE_SYSREPO:mx8mp-nxp-bsp = "real-time-edge-sysrepo-tc"
REAL_TIME_EDGE_SYSREPO:mx93-nxp-bsp = "real-time-edge-sysrepo-tc"
PACKAGECONFIG:append:pn-real-time-edge-sysrepo = "${REAL_TIME_EDGE_SYSREPO}"

# dtb for jailhouse
KERNEL_DEVICETREE:append:ls1043ardb= " freescale/fsl-ls1043a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE:append:ls1043ardb= " freescale/fsl-ls1043a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE:append:ls1046ardb= " freescale/fsl-ls1046a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE:append:ls1046ardb= " freescale/fsl-ls1046a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE:append:ls1028ardb= " freescale/fsl-ls1028a-rdb-jailhouse.dtb \
				       freescale/fsl-ls1028a-rdb-jailhouse-without-enetc.dtb \
				       freescale/fsl-ls1028a-rdb-dsa-swp5-eno3.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-root.dtb"

# opcua override default log level
LIBOPEN62541_LOGLEVE = "300"

# Set preferred version for libbpf to v0.6
PREFERRED_VERSION_libbpf = "0.6.%"
