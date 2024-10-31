include conf/distro/include/fsl-imx-base.inc
include conf/distro/include/fsl-imx-preferred-env.inc
include conf/distro/include/real-time-edge-base.inc
include conf/distro/include/igh-ethercat.inc
include conf/distro/include/libopen62541.inc
include conf/distro/include/rtos-industrial-examples.inc

# Remove conflicting backends
DISTRO_FEATURES:remove = "directfb x11 "
DISTRO_FEATURES:append = " wayland pam systemd"

# System Manager config
SYSTEM_MANAGER_CONFIG:imx95-19x19-lpddr5-evk = "mx95rte"

# boot image config
IMAGE_IMXBOOT_TARGET:imx95-19x19-lpddr5-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
