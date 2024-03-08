# Copyright 2024 NXP

REAL_TIME_EDGE_LINUX_SRC = "git://github.com/nxp-qoriq/linux.git;protocol=https"
REAL_TIME_EDGE_LINUX_BRANCH = "la12xx-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "84bd90aba15cbd08b92a35f4a6889232c8fe210d"

DPDK_SRC = "git://github.com/nxp/dpdk;protocol=https;nobranch=1"
SRCREV_dpdk_rev = "44ed2d1fbf39189c80b5876a4bdbc836799d8c29"

SRC_LA9310_HOST_URI = "git://github.com/nxp-qoriq/la93xx_host_sw;protocol=https;nobranch=1;name=host_repo"
SRCREV_host_repo = "b4c1effb8b581feb7c019763a8a83b3a32a135c6"

SRC_LA9310_FRTOS_URI = "git://github.com/nxp-qoriq/la93xx_freertos;protocol=https;nobranch=1;name=freertos_repo"
SRCREV_freertos_repo = "bd0269d851f8ba7ebeb9b7be9c88b0fa7c1009eb"
	
SRC_LA9310_FW_URI = " git://github.com/nxp-qoriq/la93xx_firmware;protocol=https;nobranch=1;name=firmware_repo"
SRCREV_firmware_repo = "c298c417f689f0f6709a94d1c75156b1ed2934f5"

SRC_LA9310_RFNM_CLI_URI = "git://git@bitbucket.sw.nxp.com/dn5g/rfnm-cli.git;protocol=ssh;branch=main;name=rfnm_cli_repo"
SRCREV_rfnm_cli_repo = "${AUTOREV}"

SRC_LA9310_GEUL_RF_UTIL_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_rf_util.git;protocol=ssh;branch=la12xx;name=geul_rf_util_repo"
SRCREV_geul_rf_util_repo = "${AUTOREV}"

SRC_LA9310_GEUL_COMMON_HEADER_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_common_headers.git;protocol=ssh;branch=la12xx;name=geul_common_headers_repo"
SRCREV_geul_common_headers_repo = "${AUTOREV}"

SRC_DPDK_EXTRAS = "git://github.com/nxp-qoriq/dpdk-extras.git;protocol=https;nobranch=1;name=dpdk-extras_repo"
SRCREV_dpdk-extras_repo = "e53931e22a15858c5469f9cb0b631f40c3ec8db3"

