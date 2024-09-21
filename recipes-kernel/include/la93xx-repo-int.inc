# Copyright 2024 NXP

PV = "1.0+git${SRCPV}"

NXP_INTERNAL = "1"

REAL_TIME_EDGE_LINUX_SRC = "git://git@bitbucket.sw.nxp.com/dn5g/5g-linux.git;protocol=ssh"
REAL_TIME_EDGE_LINUX_BRANCH = "la9310-linux-5.15-rt"
REAL_TIME_EDGE_LINUX_SRCREV = "${AUTOREV}"

REAL_TIME_EDGE_UBOOT_SRC ?= "git://git@bitbucket.sw.nxp.com/dn5g/5g-uboot.git;protocol=ssh"
REAL_TIME_EDGE_UBOOT_BRANCH = "imx8mp-la9310-v2022.04-2.5.0"
REAL_TIME_EDGE_UBOOT_SRCREV = "${AUTOREV}"

DPDK_SRC = "git://git@bitbucket.sw.nxp.com/gitam/dpdk.git;protocol=ssh;branch=22.11-qoriq-dev"
SRCREV_dpdk_rev = "${AUTOREV}"

SRC_LA9310_HOST_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/la931x_host_sw.git;protocol=ssh;branch=la12xx;name=host_repo"
SRCREV_host_repo = "${AUTOREV}"

SRC_LA9310_FRTOS_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/la931x_freertos.git;protocol=ssh;branch=la12xx;name=freertos_repo"
SRCREV_freertos_repo = "${AUTOREV}"

SRC_LA9310_FW_URI = "git://git@bitbucket.sw.nxp.com/dn5g/la931x_firmware.git;protocol=ssh;branch=master;name=firmware_repo"
SRCREV_firmware_repo = "${AUTOREV}"

SRC_LA9310_RFNM_CLI_URI = "git://git@bitbucket.sw.nxp.com/dn5g/rfnm-cli.git;protocol=ssh;branch=main;name=rfnm_cli_repo"
SRCREV_rfnm_cli_repo = "${AUTOREV}"

SRC_LA9310_GEUL_RF_UTIL_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_rf_util.git;protocol=ssh;branch=rfnm_mt3812_dev;name=geul_rf_util_repo"
SRCREV_geul_rf_util_repo = "${AUTOREV}"

SRC_LA9310_GEUL_RF_DIORA_URI = "git://git@bitbucket.sw.nxp.com/dn5g/rf_diora_sdk.git;protocol=ssh;branch=rfnm_mt3812_dev;name=geul_rf_diora_repo"
SRCREV_geul_rf_diora_repo = "${AUTOREV}"

SRC_LA9310_GEUL_COMMON_HEADER_URI = "git://git@bitbucket.sw.nxp.com/dnnpi/geul_common_headers.git;protocol=ssh;branch=la12xx;name=geul_common_headers_repo"
SRCREV_geul_common_headers_repo = "${AUTOREV}"

SRC_DPDK_EXTRAS = "git://github.com/nxp-qoriq/dpdk-extras.git;protocol=https;nobranch=1;name=dpdk-extras_repo"
SRCREV_dpdk-extras_repo = "e53931e22a15858c5469f9cb0b631f40c3ec8db3"

SRC_LMS7002M_URI = "git://github.com/rfnm/lms7002m;protocol=https;nobranch=1;name=LMS7002M_repo"
SRCREV_LMS7002M_repo = "335ca15e478948ef6762910b8c95dfb4ce0e6617"

SRC_LA9310_FREERTOS_VARISCITE_URI =  "git://github.com/varigit/freertos-variscite.git;protocol=https;branch=mcuxpresso_sdk_2.10.x-var01;name=freertos_variscite_repo"
SRCREV_freertos_variscite_repo = "1a174e74344976450dcf29275c68464b23f7110d"

SRC_LA9310_IMX8MP_M7_URI = "git://git@bitbucket.sw.nxp.com/dn5g/imx8mp-m7.git;protocol=ssh;branch=main;name=imx8mp-m7_repo"
SRCREV_imx8mp-m7_repo = "${AUTOREV}"

SRC_LA9310_FR1_FR2_TEST_TOOL_URI = "git://git@bitbucket.sw.nxp.com/dn5g/fr1_fr2_test_tool.git;protocol=ssh;branch=master;name=fr1_fr2_test_tool_repo"
SRCREV_fr1_fr2_test_tool_repo = "${AUTOREV}"
