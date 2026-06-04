require conf/distro/fsl-imx-xwayland.conf
require conf/distro/include/real-time-edge-base.inc

include conf/distro/include/igh-ethercat.inc
include conf/distro/include/libopen62541.inc

# System Manager config
SYSTEM_MANAGER_CONFIG:imx943-19x19-lpddr4-evk = "mx94rte"
SYSTEM_MANAGER_CONFIG:imx943-19x19-lpddr5-evk = "mx94rte"
SYSTEM_MANAGER_CONFIG:imx943-15x15-lpddr4-evk = "mx94rte"
SYSTEM_MANAGER_CONFIG:imx95-15x15-lpddr4x-evk = "mx95rte"
# Multiple system manager configs by IMXBOOT_VARIANT
SYSTEM_MANAGER_CONFIG:imx95-19x19-lpddr5-evk = \
    "${@bb.utils.contains('IMXBOOT_VARIANT', 'netc',      'mx95netc', \
                                                          'mx95rte', d)}"

# boot image config
IMAGE_IMXBOOT_TARGET:imx943-19x19-lpddr4-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
IMAGE_IMXBOOT_TARGET:imx943-19x19-lpddr5-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
IMAGE_IMXBOOT_TARGET:imx943-15x15-lpddr4-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
IMAGE_IMXBOOT_TARGET:imx95-15x15-lpddr4x-evk = "${IMXBOOT_TARGETS_BASENAME}_a55"
IMAGE_IMXBOOT_TARGET:imx95-19x19-lpddr5-evk = \
    "${@bb.utils.contains('IMXBOOT_VARIANT', 'netc',      '${IMXBOOT_TARGETS_BASENAME}_netc', \
                                                          '${IMXBOOT_TARGETS_BASENAME}_a55', d)}"

M4_DEFAULT_IMAGE:imx95-19x19-lpddr5-evk = \
    "${@bb.utils.contains('SYSTEM_MANAGER_CONFIG', 'mx95netc', 'netc_share_cm7.bin', \
                                                               'imx95-19x19-evk_m7_TCM_power_mode_switch.bin', d)}"

# Enable multiconfig for imx91evk machine
BBMULTICONFIG:imx91evk = "mc-imx91-9x9-qsb mc-imx91-11x11-frdm mc-imx91-11x11-frdm-imx91s"

# Enable multiconfig for imx93evk machine
BBMULTICONFIG:imx93evk = "mc-imx93-9x9-qsb mc-imx93-11x11-frdm mc-imx93-14x14-evk"

# Enable multiconfig for imx8mpevk machine
BBMULTICONFIG:imx8mpevk = "mc-imx8mp-navq mc-imx8mp-ddr4-evk mc-imx8mp-lpddr4-frdm"

# Enable multiconfig for imx943evk machine
BBMULTICONFIG:imx943evk = "mc-imx943-19x19-lpddr4-evk mc-imx943-15x15-lpddr4-evk"

# Enable multiconfig for imx95evk machine
BBMULTICONFIG:imx95evk = "mc-imx95-15x15-evk mc-imx95-15x15-frdm mc-imx95-19x19-frdm-pro"
