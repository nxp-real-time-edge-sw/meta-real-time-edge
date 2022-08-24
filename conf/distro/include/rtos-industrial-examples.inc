# arm gcc toolchain
# ARMGCC_DIR ?= "${DEPLOY_DIR_IMAGE}/arm-none-eabi"

# board name
RTOS-INDUSTRIAL-BOARD ?= ""
RTOS-INDUSTRIAL-BOARD:mx8mm-nxp-bsp = "evkmimx8mm"
# RTOS-INDUSTRIAL-BOARD:mx8mn-nxp-bsp = "evkmimx8mn"
RTOS-INDUSTRIAL-BOARD:mx8mp-nxp-bsp = "evkmimx8mp"
# RTOS-INDUSTRIAL-BOARD:mx8mq-nxp-bsp = "evkmimx8mq"

python () {
    if d.getVar('RTOS-INDUSTRIAL-BOARD', True):
        d.appendVar('DISTRO_FEATURES', " rtos-industrial" )
}

EXAMPLE_TARGET_TYPE = " \
    release \
    ddr_release \
"
