# arm gcc toolchain
# ARMGCC_DIR ?= "${DEPLOY_DIR_IMAGE}/arm-none-eabi"

# board name
RTOS-INDUSTRIAL-BOARD ?= ""
RTOS-INDUSTRIAL-BOARD:mx8mm-nxp-bsp = "evkmimx8mm"
# RTOS-INDUSTRIAL-BOARD:mx8mn-nxp-bsp = "evkmimx8mn"
RTOS-INDUSTRIAL-BOARD:mx8mp-nxp-bsp = "evkmimx8mp"
# RTOS-INDUSTRIAL-BOARD:mx8mq-nxp-bsp = "evkmimx8mq"
RTOS-INDUSTRIAL-BOARD:imx93evk = "mcimx93evk"
RTOS-INDUSTRIAL-BOARD:imx93qsb = "mcimx93qsb"

# board name for heterogeneous multicore examples
RTOS-INDUSTRIAL-BOARD-CA:mx8mm-nxp-bsp = "evkmimx8mm_ca53"
RTOS-INDUSTRIAL-BOARD-CA:mx8mp-nxp-bsp = "evkmimx8mp_ca53"
RTOS-INDUSTRIAL-BOARD-CA:imx93evk = "mcimx93evk_ca55"

RTOS-INDUSTRIAL-BOARD-CM:mx8mm-nxp-bsp = "evkmimx8mm_cm4"
RTOS-INDUSTRIAL-BOARD-CM:mx8mp-nxp-bsp = "evkmimx8mp_cm7"
RTOS-INDUSTRIAL-BOARD-CM:imx93evk = "mcimx93evk_cm33"

python () {
    if d.getVar('RTOS-INDUSTRIAL-BOARD', True):
        d.appendVar('DISTRO_FEATURES', " rtos-industrial" )
}

EXAMPLE_TARGET_TYPE = " \
    release \
    ddr_release \
"
