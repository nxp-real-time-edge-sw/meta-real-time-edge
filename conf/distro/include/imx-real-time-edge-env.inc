require conf/distro/fsl-imx-xwayland.conf
require conf/distro/include/real-time-edge-base.inc

include conf/distro/include/igh-ethercat.inc
include conf/distro/include/libopen62541.inc
include conf/distro/include/imx-fastboot-env.inc

# Multiple system manager configs by IMXBOOT_VARIANT
SYSTEM_MANAGER_CONFIG:mx943-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',             'mx94alt', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse',       'mx94evkjailhouse', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'netc',            'mx94evknetc', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'netc_standalone', 'mx94evknetc', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'netc_reset',      'mx94evknetc', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',           'mx94evkrpmsg', \
                                                                                                  'mx94rte', d), d), d), d), d),d) }"

SYSTEM_MANAGER_CONFIG:mx95-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',       'mx95alt', \
                                         bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse', 'mx95evkjailhouse', \
                                         bb.utils.contains('IMXBOOT_VARIANT', 'netc',      'mx95netc', \
                                         bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',     'mx95evkrpmsg', \
                                         bb.utils.contains('IMXBOOT_VARIANT', 'sof',       'mx95evksof', \
                                                                                           'mx95rte', d), d), d), d), d)}"

SYSTEM_MANAGER_CONFIG:mx952-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',       'mx952alt', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse', 'mx952evkjailhouse', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'netc',      'mx952evknetc', \
                                          bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',     'mx952evkrpmsg', \
                                                                                            'mx952rte', d), d), d), d)}"

# boot image config: if IMAGE_IMXBOOT_TARGET is not set, it will select the first of IMXBOOT_TARGETS
IMXBOOT_TARGETS_SD:mx943-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',             '${IMXBOOT_TARGETS_BASENAME}_a55', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse',       '${IMXBOOT_TARGETS_BASENAME}_jailhouse', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'netc',            '${IMXBOOT_TARGETS_BASENAME}_all', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'netc_standalone', '${IMXBOOT_TARGETS_BASENAME}_all', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'netc_reset',      '${IMXBOOT_TARGETS_BASENAME}_all', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',           '${IMXBOOT_TARGETS_BASENAME}_a55', \
                                                                                               '${IMXBOOT_TARGETS_BASENAME}_a55 ${IMXBOOT_TARGETS_BASENAME}_all', d), d), d), d), d), d)} \
"

IMXBOOT_TARGETS_SD:mx95-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',       '${IMXBOOT_TARGETS_BASENAME}_alt', \
                                      bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse', '${IMXBOOT_TARGETS_BASENAME}_jailhouse', \
                                      bb.utils.contains('IMXBOOT_VARIANT', 'netc',      '${IMXBOOT_TARGETS_BASENAME}_netc', \
                                      bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',     '${IMXBOOT_TARGETS_BASENAME}_lpboot_sm_a55', \
                                      bb.utils.contains('IMXBOOT_VARIANT', 'sof',       '${IMXBOOT_TARGETS_BASENAME}_a55', \
                                                                                        '${IMXBOOT_TARGETS_BASENAME}_a55 ${IMXBOOT_TARGETS_BASENAME}_all', d), d), d), d), d)} \
"

IMXBOOT_TARGETS_SD:mx952-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'alt',       '${IMXBOOT_TARGETS_BASENAME}_all', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'jailhouse', '${IMXBOOT_TARGETS_BASENAME}_jailhouse', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'netc',      '${IMXBOOT_TARGETS_BASENAME}_all', \
                                       bb.utils.contains('IMXBOOT_VARIANT', 'rpmsg',     '${IMXBOOT_TARGETS_BASENAME}_lpboot_sm_a55', \
                                                                                         '${IMXBOOT_TARGETS_BASENAME}_a55 ${IMXBOOT_TARGETS_BASENAME}_all', d), d), d), d)} \
"

# For the "netc" variant, i.MX943 must use the netc-share Cortex-M33 firmware
# built by the netc-share HMC recipe (meta-rtos-industrial) instead of the
# default BSP mcore-demos binary. That firmware is deployed under
# ${DEPLOY_DIR_IMAGE}/examples/${RTOS_MACHINE}/heterogeneous-multicore/netc-share-freertos/.
# imx-boot copies M33_IMAGE relative to ${DEPLOY_DIR_IMAGE}/mcore-demos, so a
# "../examples/..." relative path resolves to the netc-share deploy location.
# The other variants keep the standard BSP mapping (see imx943-evk.inc).
M33_IMAGE:mx943-nxp-bsp = "${@bb.utils.contains('IMXBOOT_VARIANT', 'netc',            '../examples/${RTOS_MACHINE}/heterogeneous-multicore/netc-share-freertos/netc_share_cm33_core1.bin', \
                              bb.utils.contains('IMXBOOT_VARIANT', 'netc_reset',      'imx943evk_cm33_core1_TCM_all_reset_but_netc_switch.bin', \
                              bb.utils.contains('IMXBOOT_VARIANT', 'netc_standalone', 'imx943evk_cm33_core1_TCM_netc_switch_standalone.bin', \
                                                                                     'imx943evk_cm33_core1_TCM_power_mode_switch.bin', d), d), d)}"

# For the "netc" variant, i.MX95 19x19 LPDDR5 EVK must use the netc-share
# Cortex-M7 firmware built by the netc-share HMC recipe (meta-rtos-industrial)
# instead of the default BSP mcore-demos binary. That firmware is deployed under
# ${DEPLOY_DIR_IMAGE}/examples/${RTOS_MACHINE}/heterogeneous-multicore/netc-share-freertos/.
# The BSP resolves M4_DEFAULT_IMAGE:mx95-nxp-bsp = "${M4_DEFAULT_IMAGE_MX95}", so
# we override the leaf M4_DEFAULT_IMAGE_MX95 (matching the BSP machine-conf style)
# scoped to imx95-19x19-lpddr5-evk only. imx-boot copies M4_DEFAULT_IMAGE relative
# to ${DEPLOY_DIR_IMAGE}/mcore-demos, so a "../examples/..." relative path resolves
# to the netc-share deploy location.
M4_DEFAULT_IMAGE_MX95:imx95-19x19-lpddr5-evk = \
    "${@bb.utils.contains('IMXBOOT_VARIANT', 'netc', '../examples/${RTOS_MACHINE}/heterogeneous-multicore/netc-share-freertos/netc_share_cm7.bin', \
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
