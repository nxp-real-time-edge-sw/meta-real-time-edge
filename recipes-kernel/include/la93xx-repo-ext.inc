# Copyright 2024 NXP

NXP_INTERNAL = "0"

REAL_TIME_EDGE_LINUX_SRC = "git://github.com/nxp-qoriq/linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH = "la9310-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "d462c158a0ea41b37c10ab32bb34688223314a53"

REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-qoriq/u-boot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "imx8mp-la9310-v2022.04-2.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "356e9341c9b3c326d33037d3d36d9c0fbe031368"

DPDK_SRC = "git://github.com/nxp-qoriq/dpdk;protocol=https;nobranch=1"
SRCREV_dpdk_rev = "d48d9a4a045eef1d4ef4b8fbccfe5107e1293a5e"

SRC_LA9310_HOST_URI = "git://github.com/nxp-qoriq/la93xx_host_sw;protocol=https;nobranch=1;name=host_repo"
SRCREV_host_repo = "32445931971b2fb1c65b84872c23d6c6769fe0a4"

SRC_LA9310_FRTOS_URI = "git://github.com/nxp-qoriq/la93xx_freertos;protocol=https;nobranch=1;name=freertos_repo"
SRCREV_freertos_repo = "67a8fcde0f9f51b7ade567556dba0c08c7465fbf"
	
SRC_LA9310_FW_URI = " git://github.com/nxp-qoriq/la93xx_firmware;protocol=https;nobranch=1;name=firmware_repo"
SRCREV_firmware_repo = "5681debf6e0240f33ae930adda5808e577c328a7"

SRC_DPDK_EXTRAS = "git://github.com/nxp-qoriq/dpdk-extras.git;protocol=https;nobranch=1;name=dpdk-extras_repo"
SRCREV_dpdk-extras_repo = "e53931e22a15858c5469f9cb0b631f40c3ec8db3"

SRC_LMS7002M_URI = "git://github.com/rfnm/lms7002m;protocol=https;nobranch=1;name=LMS7002M_repo"
SRCREV_LMS7002M_repo = "335ca15e478948ef6762910b8c95dfb4ce0e6617"

SRC_LIMESUITENG_URI = "git://github.com/ehntoo/LimeSuiteNG.git;protocol=https;branch=rfnm-less-malloc;name=LIMESUITENG_repo"
SRCREV_LIMESUITENG_repo = "bbd08c2af32f3f3e5d0b009c5634c716dceed213"

SRC_LA9310_FREERTOS_VARISCITE_URI =  "git://github.com/varigit/freertos-variscite.git;protocol=https;branch=mcuxpresso_sdk_2.10.x-var01;name=freertos_variscite_repo"
SRCREV_freertos_variscite_repo = "1a174e74344976450dcf29275c68464b23f7110d"

SRC_LA9310_IMX8MP_M7_URI = "git://github.com/rfnm/imx8mp-m7.git;protocol=https;branch=main;name=imx8mp-m7_repo"
SRCREV_imx8mp-m7_repo = "cdf07e8d249526c09ce471cc14658d096862e3e8"
