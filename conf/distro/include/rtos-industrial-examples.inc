# arm gcc toolchain
# ARMGCC_DIR ?= "${DEPLOY_DIR_IMAGE}/arm-none-eabi"

# board name
RTOS-INDUSTRIAL-BOARD ?= ""
RTOS-INDUSTRIAL-BOARD:mx8mm-nxp-bsp = "evkmimx8mm"
# RTOS-INDUSTRIAL-BOARD:mx8mn-nxp-bsp = "evkmimx8mn"
RTOS-INDUSTRIAL-BOARD:mx8mp-nxp-bsp = "evkmimx8mp"
# RTOS-INDUSTRIAL-BOARD:mx8mq-nxp-bsp = "evkmimx8mq"
RTOS-INDUSTRIAL-BOARD:imx93evk = "mcimx93evk"
RTOS-INDUSTRIAL-BOARD:imx93-9x9-lpddr4-qsb = "mcimx93qsb"
RTOS-INDUSTRIAL-BOARD:imx93-14x14-lpddr4x-evk = "mcimx93autoevk"
RTOS-INDUSTRIAL-BOARD:imx95-15x15-lpddr4x-evk = "imx95lpd4xevk15"
RTOS-INDUSTRIAL-BOARD:imx95-19x19-lpddr5-evk = "imx95lpd5evk19"
RTOS-INDUSTRIAL-BOARD:imx91-9x9-lpddr4-qsb = "mcimx91qsb"
RTOS-INDUSTRIAL-BOARD:imx91-11x11-lpddr4-evk = "mcimx91evk"
RTOS-INDUSTRIAL-BOARD:mx943-nxp-bsp = "imx943evk"

# board name for heterogeneous multicore examples
RTOS-INDUSTRIAL-BOARD-CA:mx8mm-nxp-bsp = "evkmimx8mm_ca53"
RTOS-INDUSTRIAL-BOARD-CA:mx8mp-nxp-bsp = "evkmimx8mp_ca53"
RTOS-INDUSTRIAL-BOARD-CA:imx93evk = "mcimx93evk_ca55"
RTOS-INDUSTRIAL-BOARD-CA:imx93-14x14-lpddr4x-evk = "mcimx93evk_ca55"
RTOS-INDUSTRIAL-BOARD-CA:imx95-15x15-lpddr4x-evk = "mcimx95evk_ca55"
RTOS-INDUSTRIAL-BOARD-CA:imx95-19x19-lpddr5-evk = "mcimx95evk_ca55"
RTOS-INDUSTRIAL-BOARD-CA:imx91-9x9-lpddr4-qsb = "mcimx91qsb_ca55"
RTOS-INDUSTRIAL-BOARD-CA:imx91-11x11-lpddr4-evk = "mcimx91evk_ca55"

RTOS-INDUSTRIAL-BOARD-CM:mx8mm-nxp-bsp = "evkmimx8mm_cm4"
RTOS-INDUSTRIAL-BOARD-CM:mx8mp-nxp-bsp = "evkmimx8mp_cm7"
RTOS-INDUSTRIAL-BOARD-CM:imx93evk = "mcimx93evk_cm33"
RTOS-INDUSTRIAL-BOARD-CM:imx93-14x14-lpddr4x-evk = "mcimx93evk_cm33"

python () {
    if d.getVar('RTOS-INDUSTRIAL-BOARD', True):
        d.appendVar('DISTRO_FEATURES', " rtos-industrial" )
}

EXAMPLE_TARGET_TYPE = " \
    release \
    ddr_release \
"
