# Real-time Edge base conf

DISTRO = "nxp-real-time-edge"
DISTROOVERRIDES:append = " :real-time-edge"
DISTRO_NAME = "NXP Real-time Edge Distro"
DISTRO_VERSION = "3.0"

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
DISTRO_FEATURES:append:ls1021atsn = " real-time-edge-sysrepo tsn-scripts real-time-edge-prl"
DISTRO_FEATURES:append:ls1021aiot = " canfestival"

# For imx8dxlevk remove jailhouse feature until the board is added into jailhouse.
DISTRO_FEATURES:remove:mx8dxl-nxp-bsp = " jailhouse"

# For ls1028ardb
DISTRO_FEATURES:remove:ls1028ardb = " x11"

PREFERRED_VERSION_imx-gpu-viv:ls1028a = "6.4.11.p2.8-aarch64"
PREFERRED_VERSION_weston:ls1028ardb = "12.0.4.imx"
PREFERRED_VERSION_wayland-protocols:ls1028ardb = "1.32.imx"
PREFERRED_VERSION_libdrm:ls1028ardb = "2.4.116.imx"

PREFERRED_PROVIDER_virtual/mesa:ls1028a   = "mesa-gl"
PREFERRED_PROVIDER_virtual/libgl:ls1028a  = "mesa-gl"
PREFERRED_PROVIDER_virtual/libgles1:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles2:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/libgles3:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/egl:ls1028ardb  = "imx-gpu-viv"
PREFERRED_PROVIDER_virtual/opencl-icd:ls1028ardb = "imx-gpu-viv"

LICENSE_FLAGS_ACCEPTED:append:ls1028ardb = " commercial"
MACHINEOVERRIDES:append:ls1028ardb = ":imxdrm"

DELTA_KERNEL_DEFCONFIG:append:mx8mm-nxp-bsp = "${@bb.utils.contains('DISTRO_FEATURES', 'rpmsg_8m_buf', \
		' linux-rpmsg-8m-buf.config', ' ', d)}"
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
					       freescale/imx8mm-evk-virtio-net-ca53.dtb \
					       freescale/imx8mm-evk-multicore-rtos.dtb \
					       freescale/imx8mm-evk-multicore-rpmsg.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-rpmsg-8m-buf.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-root.dtb \
					       freescale/imx8mp-evk-rpmsg-ca53.dtb \
					       freescale/imx8mp-evk-virtio-net-cm7.dtb \
					       freescale/imx8mp-evk-virtio-perf-cm7.dtb \
					       freescale/imx8mp-evk-virtio-net-ca53.dtb \
					       freescale/imx8mp-evk-virtio-perf-ca53.dtb \
					       freescale/imx8mp-evk-multicore-rtos.dtb \
					       freescale/imx8mp-evk-multicore-lwip.dtb \
					       freescale/imx8mp-evk-multicore-rpmsg.dtb \
					       freescale/imx8mp-evk-dsa.dtb \
					       freescale/imx8mp-evk-revb4-dsa.dtb \
					       freescale/imx8mp-evk-dsa-enetc.dtb \
					       freescale/imx8mp-evk-revb4-dsa-enetc.dtb \
					       freescale/imx8mp-evk-revb4-dsa-fec-swp0.dtb \
					       freescale/imx8mp-evk-revb4-dsa-fec-swp3.dtb "
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-uart-sharing-cm33.dtb \
				      freescale/imx93-11x11-evk-virtio-net-ca55.dtb \
				      freescale/imx93-11x11-evk-virtio-net-cm33.dtb \
				      freescale/imx93-11x11-evk-dsa.dtb \
				      freescale/imx93-11x11-evk-dsa-enetc.dtb \
				      freescale/imx93-11x11-evk-dsa-fec-swp0.dtb \
				      freescale/imx93-11x11-evk-dsa-fec-swp3.dtb \
				      freescale/imx93-11x11-evk-dpdk.dtb \
				      freescale/imx93-11x11-evk-multicore-rtos.dtb"
KERNEL_DEVICETREE:append:imx93-9x9-lpddr4-qsb = " freescale/imx93-9x9-qsb-inmate.dtb \
						  freescale/imx93-9x9-qsb-root.dtb \
						  freescale/imx93-9x9-qsb-uart-sharing-cm33.dtb"

KERNEL_DEVICETREE:append:imx93-14x14-lpddr4x-evk = " freescale/imx93-14x14-evk-uart-sharing-cm33.dtb \
						     freescale/imx93-14x14-evk-virtio-net-ca55.dtb \
						     freescale/imx93-14x14-evk-virtio-net-cm33.dtb \
						     freescale/imx93-14x14-evk-multicore-rtos.dtb"

KERNEL_DEVICETREE:append:imx95-19x19-lpddr5-evk = " freescale/imx95-19x19-evk-multicore-rtos.dtb"
KERNEL_DEVICETREE:append:imx95-15x15-lpddr4x-evk = " freescale/imx95-15x15-evk-multicore-rtos.dtb"

# dtb for igh
KERNEL_DEVICETREE:append:imx6ull14x14evk = " imx6ull-14x14-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-igh.dtb"

# dtb for plc
KERNEL_DEVICETREE:append:imx6ull14x14evk = " imx6ull-14x14-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-ecat.dtb"

# dtb for EtherCAT userspace application
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-ecat-userspace.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-ecat-userspace.dtb"
KERNEL_DEVICETREE:append:imx93evk = " freescale/imx93-11x11-evk-ecat-userspace.dtb"

# dtb for lpuart
KERNEL_DEVICETREE:append:imx6ull14x14evk = " imx6ull-14x14-evk-lpuart.dtb"

# dtb for imxai2eth-ath
KERNEL_DEVICETREE:append:imx93-14x14-lpddr4x-evk = " freescale/imx93-14x14-evk-imxai2eth-ath.dtb"

# opcua override default log level
LIBOPEN62541_LOGLEVE = "300"
