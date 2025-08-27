require conf/distro/fsl-imx-xwayland.conf
require conf/distro/include/real-time-edge-base.inc

include conf/distro/include/igh-ethercat.inc
include conf/distro/include/libopen62541.inc
include conf/distro/include/rtos-industrial-examples.inc

# System Manager config
SYSTEM_MANAGER_CONFIG:imx95-19x19-lpddr5-evk = "mx95rte"
SYSTEM_MANAGER_CONFIG:imx95-15x15-lpddr4x-evk = "mx95rte"

# boot image config
IMAGE_IMXBOOT_TARGET:imx95-19x19-lpddr5-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
IMAGE_IMXBOOT_TARGET:imx95-15x15-lpddr4x-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
