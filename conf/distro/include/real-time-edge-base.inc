# Real-time Edge base conf

DISTRO = "nxp-real-time-edge"
DISTROOVERRIDES = "real-time-edge"
DISTRO_NAME = "NXP Real-time Edge Distro"
DISTRO_VERSION = "2.7"

CONNECTIVITY_CHECK_URIS ?= "https://yoctoproject.org/connectivity.html"

DISTRO_FEATURES:append:ls1043ardb = " jailhouse"
DISTRO_FEATURES:append:ls1046ardb = " jailhouse"
DISTRO_FEATURES:append:ls1028ardb = " jailhouse real-time-edge-libbee \
				      real-time-edge-libblep libnfc-nci \
				      wayland-protocols weston imx-gpu-viv libdrm kmscube \
				      real-time-edge-sysrepo tsn-scripts wayland alsa"
DISTRO_FEATURES:append:mx8dxl-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:mx8mp-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:mx8m-nxp-bsp = " wayland-protocols weston imx-gpu-viv libdrm kmscube modbus-simulator"
DISTRO_FEATURES:append:mx93-nxp-bsp = " real-time-edge-sysrepo"
DISTRO_FEATURES:append:ls1021atsn = " real-time-edge-sysrepo feedgnuplot tsn-scripts real-time-edge-prl"
DISTRO_FEATURES:append:ls1021aiot = " canfestival"

# For imx8dxlevk remove jailhouse feature until the board is added into jailhouse.
DISTRO_FEATURES:remove:mx8dxl-nxp-bsp = " jailhouse"

# For ls1028ardb
DISTRO_FEATURES:remove:ls1028ardb = " x11"
PREFERRED_VERSION_weston:ls1028ardb = "11.0.2.imx"
PREFERRED_VERSION_wayland-protocols:ls1028ardb = "1.32.imx"
PREFERRED_VERSION_libdrm:ls1028ardb = "2.4.115.imx"
PREFERRED_PROVIDER_virtual/libgl:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles1:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/egl:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/opencl-icd:ls1028ardb = "imx-gpu-viv"
LICENSE_FLAGS_ACCEPTED:append:ls1028ardb = " commercial"
DISTROOVERRIDES:append:ls1028ardb = ":imxdrm"

DELTA_KERNEL_DEFCONFIG:append:mx8mm-nxp-bsp = "${@bb.utils.contains('DISTRO_FEATURES', 'rpmsg_8m_buf', \
		' linux-rpmsg-8m-buf.config', ' ', d)}"
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
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-rpmsg-ca53.dtb \
					       freescale/imx8mm-evk-virtio-perf-cm4.dtb \
					       freescale/imx8mm-evk-virtio-net-cm4.dtb \
					       freescale/imx8mm-evk-virtio-perf-ca53.dtb \
					       freescale/imx8mm-evk-virtio-net-ca53.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-rpmsg-8m-buf.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-root.dtb \
					       freescale/imx8mp-evk-virtio-net-cm7.dtb \
					       freescale/imx8mp-evk-virtio-net-ca53.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-uart-sharing-cm33.dtb \
				      freescale/imx93-11x11-evk-virtio-net-ca55.dtb \
				      freescale/imx93-11x11-evk-virtio-net-cm33.dtb"

# dtb for plc
KERNEL_DEVICETREE:append:imx6ull14x14evk = " imx6ull-14x14-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-ecat.dtb"

# opcua override default log level
LIBOPEN62541_LOGLEVE = "300"

# Set preferred version for libbpf to v0.6
PREFERRED_VERSION_libbpf = "0.6.%"
