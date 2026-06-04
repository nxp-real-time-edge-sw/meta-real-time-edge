# i.MX93 EVK Multi-machine Examples Support

IMX93EVK_MACHINE = "imx93evk"
IMX93EVK_MC_MAP = "mc-imx93-9x9-qsb:imx93-9x9-lpddr4-qsb mc-imx93-11x11-frdm:imx93-11x11-lpddr4x-frdm mc-imx93-14x14-evk:imx93-14x14-lpddr4x-evk"

python() {
    add_mcdepends_with_params(d, d.getVar('IMX93EVK_MACHINE'), d.getVar('IMX93EVK_MC_MAP'))
}

imx93evk_install_examples() {
    install_examples_with_params "${IMX93EVK_MACHINE}" "${IMX93EVK_MC_MAP}"
}

ROOTFS_POSTPROCESS_COMMAND:append:imx93evk = " imx93evk_install_examples;"
