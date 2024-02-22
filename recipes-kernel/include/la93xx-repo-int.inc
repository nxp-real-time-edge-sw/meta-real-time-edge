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
