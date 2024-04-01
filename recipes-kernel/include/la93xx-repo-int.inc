# Copyright 2024 NXP

PV = "1.0+git${SRCPV}"

REAL_TIME_EDGE_LINUX_SRC = "git://git@bitbucket.sw.nxp.com/dn5g/5g-linux.git;protocol=ssh"
REAL_TIME_EDGE_LINUX_BRANCH = "linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "${AUTOREV}"

DPDK_SRC = "git://git@bitbucket.sw.nxp.com/gitam/dpdk.git;protocol=ssh;branch=22.11-qoriq"
SRCREV_dpdk_rev = "${AUTOREV}"

SRC_LA9310_HOST_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/la931x_host_sw.git;protocol=ssh;branch=la12xx;name=host_repo"
SRCREV_host_repo = "${AUTOREV}"

SRC_LA9310_FRTOS_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/la931x_freertos.git;protocol=ssh;branch=la12xx;name=freertos_repo"
SRCREV_freertos_repo = "${AUTOREV}"

SRC_LA9310_FW_URI = "git://git@bitbucket.sw.nxp.com/dn5g/la931x_firmware.git;protocol=ssh;branch=master;name=firmware_repo"
SRCREV_firmware_repo = "${AUTOREV}"

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
