# Copyright 2024 NXP

REAL_TIME_EDGE_LINUX_SRC = "git://github.com/nxp-qoriq/linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH = "la12xx-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "5d1547599daf00fa61865a2db5344b9f3a0a09e2"

DPDK_SRC = "git://github.com/nxp/dpdk;protocol=https;nobranch=1"
SRCREV_dpdk_rev = "ca39e88005a529e481466ed0e8fef7dc3a9f0c94"

SRC_LA9310_HOST_URI = "git://github.com/nxp-qoriq/la93xx_host_sw;protocol=https;nobranch=1;name=host_repo"
SRCREV_host_repo = "b242845c01e8a9f2be586ffe48f4561990367034"

SRC_LA9310_FRTOS_URI = "git://github.com/nxp-qoriq/la93xx_freertos;protocol=https;nobranch=1;name=freertos_repo"
SRCREV_freertos_repo = "8f7752e007d43045d19f6c173812a0fabc415745"
	
SRC_LA9310_FW_URI = " git://github.com/nxp-qoriq/la93xx_firmware;protocol=https;nobranch=1;name=firmware_repo"
SRCREV_firmware_repo = "f064ade381c87ce258752ebf0cd3d9d1aadab45c"

SRC_LA9310_RFNM_CLI_URI = "git://git@bitbucket.sw.nxp.com/dn5g/rfnm-cli.git;protocol=ssh;branch=main;name=rfnm_cli_repo"
SRCREV_rfnm_cli_repo = "${AUTOREV}"

SRC_LA9310_GEUL_RF_UTIL_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_rf_util.git;protocol=ssh;branch=la12xx;name=geul_rf_util_repo"
SRCREV_geul_rf_util_repo = "${AUTOREV}"

SRC_LA9310_GEUL_COMMON_HEADER_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_common_headers.git;protocol=ssh;branch=la12xx;name=geul_common_headers_repo"
SRCREV_geul_common_headers_repo = "${AUTOREV}"

SRC_DPDK_EXTRAS = "git://github.com/nxp-qoriq/dpdk-extras.git;protocol=https;nobranch=1;name=dpdk-extras_repo"
SRCREV_dpdk-extras_repo = "e53931e22a15858c5469f9cb0b631f40c3ec8db3"

SRC_LMS7002M_URI = "git://github.com/rfnm/lms7002m;protocol=https;nobranch=1;name=LMS7002M_repo"
SRCREV_LMS7002M_repo = "335ca15e478948ef6762910b8c95dfb4ce0e6617"
