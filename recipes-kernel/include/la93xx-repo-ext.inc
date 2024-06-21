# Copyright 2024 NXP

NXP_INTERNAL = "0"

REAL_TIME_EDGE_LINUX_SRC = "git://github.com/nxp-qoriq/linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH = "la9310-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "a0d2ad11f9b0a140eb16a3c9b7f516d5d0bd926b"

REAL_TIME_EDGE_UBOOT_SRC ?= "git://github.com/nxp-qoriq/u-boot.git;protocol=https"
REAL_TIME_EDGE_UBOOT_BRANCH ?= "imx8mp-la9310-v2022.04-2.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV ?= "5a69b5b424864765628d3f3651d58b517e85ee12"

DPDK_SRC = "git://github.com/nxp/dpdk;protocol=https;nobranch=1"
SRCREV_dpdk_rev = "e5ff5bfba993da085f08df51e3924bca0bab30b7"

SRC_LA9310_HOST_URI = "git://github.com/nxp-qoriq/la93xx_host_sw;protocol=https;nobranch=1;name=host_repo"
SRCREV_host_repo = "72100bf3fb7a5ccdb1e83d02db481e251f0f660d"

SRC_LA9310_FRTOS_URI = "git://github.com/nxp-qoriq/la93xx_freertos;protocol=https;nobranch=1;name=freertos_repo"
SRCREV_freertos_repo = "c7b3fc7381fd75cdc3dc951a5125c31dd1d66a13"
	
SRC_LA9310_FW_URI = " git://github.com/nxp-qoriq/la93xx_firmware;protocol=https;nobranch=1;name=firmware_repo"
SRCREV_firmware_repo = "38e4405786f740c93b57b4f58870a1ea13abfaff"

SRC_DPDK_EXTRAS = "git://github.com/nxp-qoriq/dpdk-extras.git;protocol=https;nobranch=1;name=dpdk-extras_repo"
SRCREV_dpdk-extras_repo = "e53931e22a15858c5469f9cb0b631f40c3ec8db3"

SRC_LMS7002M_URI = "git://github.com/rfnm/lms7002m;protocol=https;nobranch=1;name=LMS7002M_repo"
SRCREV_LMS7002M_repo = "335ca15e478948ef6762910b8c95dfb4ce0e6617"

SRC_LA9310_FREERTOS_VARISCITE_URI =  "git://github.com/varigit/freertos-variscite.git;protocol=https;branch=mcuxpresso_sdk_2.10.x-var01;name=freertos_variscite_repo"
SRCREV_freertos_variscite_repo = "1a174e74344976450dcf29275c68464b23f7110d"

SRC_LA9310_IMX8MP_M7_URI = "git://github.com/rfnm/imx8mp-m7.git;protocol=https;branch=main;name=imx8mp-m7_repo"
SRCREV_imx8mp-m7_repo = "cdf07e8d249526c09ce471cc14658d096862e3e8"
