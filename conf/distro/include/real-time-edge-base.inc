# Real-time Edge base conf

DISTRO = "nxp-real-time-edge"
DISTROOVERRIDES:append = ":real-time-edge"
DISTRO_NAME = "NXP Real-time Edge Distro"
DISTRO_VERSION = "3.5"

CONNECTIVITY_CHECK_URIS ?= "https://yoctoproject.org/connectivity.html"

DISTRO_FEATURES:append:ls1043ardb = " jailhouse"
DISTRO_FEATURES:append:ls1046ardb = " jailhouse"
DISTRO_FEATURES:append:ls1028ardb = " jailhouse real-time-edge-libbee \
				      real-time-edge-libblep libnfc-nci \
				      real-time-edge-sysrepo alsa genavb-tsn"
DISTRO_FEATURES:append:mx8dxl-nxp-bsp = " real-time-edge-sysrepo genavb-tsn"
DISTRO_FEATURES:append:mx8mp-nxp-bsp = " real-time-edge-sysrepo genavb-tsn"
DISTRO_FEATURES:append:mx8m-nxp-bsp = " modbus-simulator"
DISTRO_FEATURES:append:mx93-nxp-bsp = " real-time-edge-sysrepo genavb-tsn"
DISTRO_FEATURES:append:mx95-nxp-bsp = " modbus-simulator"
DISTRO_FEATURES:append:ls1021atsn = " real-time-edge-sysrepo real-time-edge-prl"
DISTRO_FEATURES:append:ls1021aiot = " canfestival"

# For imx8dxlevk remove jailhouse feature until the board is added into jailhouse.
DISTRO_FEATURES:remove:mx8dxl-nxp-bsp = " jailhouse"

# For i.MX943
DISTRO_FEATURES:append:mx943-nxp-bsp = " real-time-edge-sysrepo genavb-tsn"

# DELTA_KERNEL_DEFCONFIG:append:mx8mm-nxp-bsp = "${@bb.utils.contains('DISTRO_FEATURES', 'rpmsg_8m_buf', \
# 		' linux-rpmsg-8m-buf.config', ' ', d)}"
# DELTA_KERNEL_DEFCONFIG:append:ls1043ardb = " linux-dpaa-ethercat.config"
# DELTA_KERNEL_DEFCONFIG:append:ls1046ardb = " linux-dpaa-ethercat.config"

REAL_TIME_EDGE_SYSREPO ?= "real-time-edge-sysrepo-tc"
REAL_TIME_EDGE_SYSREPO:ls1028ardb = ""
PACKAGECONFIG:append:pn-real-time-edge-sysrepo = "${REAL_TIME_EDGE_SYSREPO}"

# dtb for jailhouse
KERNEL_DEVICETREE:append:ls1043ardb = " freescale/fsl-ls1043a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE:append:ls1043ardb = " freescale/fsl-ls1043a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE:append:ls1046ardb = " freescale/fsl-ls1046a-rdb-sdk-jailhouse.dtb"
KERNEL_DEVICETREE:append:ls1046ardb = " freescale/fsl-ls1046a-rdb-sdk-jailhouse-with-dpaa.dtb"
KERNEL_DEVICETREE:append:ls1028ardb = " freescale/fsl-ls1028a-rdb-jailhouse.dtb \
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
# i.MX93 11x11 EVK is the primary board of the consolidated imx93evk machine.
# Its board override imx93-11x11-lpddr4x-evk is active for both the consolidated
# build (via MACHINEOVERRIDES in imx93evk.conf) and the dedicated 11x11 build, so
# attach the primary-board feature dtbs to the board override (not imx93evk).
KERNEL_DEVICETREE:append:imx93-11x11-lpddr4x-evk = " freescale/imx93-11x11-evk-uart-sharing-cm33.dtb \
				      freescale/imx93-11x11-evk-virtio-net-ca55.dtb \
				      freescale/imx93-11x11-evk-virtio-net-cm33.dtb \
				      freescale/imx93-11x11-evk-dsa.dtb \
				      freescale/imx93-11x11-evk-dsa-enetc.dtb \
				      freescale/imx93-11x11-evk-dsa-fec-swp0.dtb \
				      freescale/imx93-11x11-evk-dsa-fec-swp3.dtb \
				      freescale/imx93-11x11-evk-dpdk.dtb \
				      freescale/imx93-11x11-evk-multicore-rpmsg.dtb \
				      freescale/imx93-11x11-evk-multicore-rtos.dtb"
# RTE feature device trees for the i.MX93 9x9 QSB variant.
# The consolidated imx93evk machine bundles the 9x9 variant, but its active
# overrides are imx93-11x11-lpddr4x-evk/mx93 (see imx93evk.conf), not
# imx93-9x9-lpddr4-qsb. Append for both the consolidated machine and the
# dedicated 9x9 build so the dtbs are present in either case.
IMX93_9X9_RTE_DTB = " freescale/imx93-9x9-qsb-inmate.dtb \
		      freescale/imx93-9x9-qsb-root.dtb \
		      freescale/imx93-9x9-qsb-uart-sharing-cm33.dtb"
KERNEL_DEVICETREE:append:imx93evk = " ${IMX93_9X9_RTE_DTB}"
KERNEL_DEVICETREE:append:imx93-9x9-lpddr4-qsb = " ${IMX93_9X9_RTE_DTB}"

# RTE feature device trees for the i.MX93 14x14 EVK variant.
# Same rationale as the 9x9 block above: the consolidated imx93evk machine does
# not enable the imx93-14x14-lpddr4x-evk override, so append for both.
IMX93_14X14_RTE_DTB = " freescale/imx93-14x14-evk-uart-sharing-cm33.dtb \
			freescale/imx93-14x14-evk-virtio-net-ca55.dtb \
			freescale/imx93-14x14-evk-virtio-net-cm33.dtb \
			freescale/imx93-14x14-evk-multicore-rpmsg.dtb \
			freescale/imx93-14x14-evk-multicore-rtos.dtb"
KERNEL_DEVICETREE:append:imx93evk = " ${IMX93_14X14_RTE_DTB}"
KERNEL_DEVICETREE:append:imx93-14x14-lpddr4x-evk = " ${IMX93_14X14_RTE_DTB}"

KERNEL_DEVICETREE:append:mx943 = " freescale/imx943-evk-multicore-rtos.dtb \
				   freescale/imx943-evk-multicore-rpmsg.dtb"
KERNEL_DEVICETREE:append:imx95-19x19-lpddr5-evk = " freescale/imx95-19x19-evk-multicore-rtos.dtb \
						    freescale/imx95-19x19-evk-multicore-rpmsg.dtb"
# RTE multicore device trees for the i.MX95 15x15 EVK variant.
# The consolidated imx95evk machine bundles the 15x15 variant, but its active
# overrides are imx95-19x19-lpddr5-evk/mx95 (see imx95evk.conf), not
# imx95-15x15-lpddr4x-evk. Append for both the consolidated machine and the
# dedicated 15x15 build so the dtbs are present in either case.
IMX95_15X15_MULTICORE_DTB = " freescale/imx95-15x15-evk-multicore-rtos.dtb \
			      freescale/imx95-15x15-evk-multicore-rpmsg.dtb"
KERNEL_DEVICETREE:append:imx95evk = " ${IMX95_15X15_MULTICORE_DTB}"
KERNEL_DEVICETREE:append:imx95-15x15-lpddr4x-evk = " ${IMX95_15X15_MULTICORE_DTB}"

# dtb for igh
KERNEL_DEVICETREE:append:imx6ull14x14evk = " nxp/imx/imx6ull-14x14-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-igh.dtb"
KERNEL_DEVICETREE:append:imx93-11x11-lpddr4x-evk = " freescale/imx93-11x11-evk-igh.dtb"

# dtb for plc
KERNEL_DEVICETREE:append:imx6ull14x14evk = " nxp/imx/imx6ull-14x14-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-ecat.dtb"
KERNEL_DEVICETREE:append:imx93-11x11-lpddr4x-evk = " freescale/imx93-11x11-evk-ecat.dtb"

# dtb for EtherCAT userspace application
KERNEL_DEVICETREE:append:imx8mm-lpddr4-evk = " freescale/imx8mm-evk-ecat-userspace.dtb"
KERNEL_DEVICETREE:append:imx8mp-lpddr4-evk = " freescale/imx8mp-evk-ecat-userspace.dtb"
KERNEL_DEVICETREE:append:imx93-11x11-lpddr4x-evk = " freescale/imx93-11x11-evk-ecat-userspace.dtb"

# dtb for lpuart
# imx95-15x15-evk-lpuart is part of the consolidated imx95evk image, whose active
# overrides do not include imx95-15x15-lpddr4x-evk, so append for both.
KERNEL_DEVICETREE:append:imx6ull14x14evk = " nxp/imx/imx6ull-14x14-evk-lpuart.dtb"
KERNEL_DEVICETREE:append:imx95evk = " freescale/imx95-15x15-evk-lpuart.dtb"
KERNEL_DEVICETREE:append:imx95-15x15-lpddr4x-evk = " freescale/imx95-15x15-evk-lpuart.dtb"

# dtb for DSA
KERNEL_DEVICETREE:append:mx943 = " freescale/imx943-evk-hms-dsa.dtb"

# opcua override default log level
LIBOPEN62541_LOGLEVE = "300"

PREFERRED_PROVIDER_virtual/libgles3:ls1028ardb  = "imx-gpu-viv"
