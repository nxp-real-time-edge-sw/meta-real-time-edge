# i.MX95 EVK Multi-machine Examples Support

IMX95EVK_MACHINE = "imx95evk"
IMX95EVK_MC_MAP = "mc-imx95-15x15-evk:imx95-15x15-lpddr4x-evk mc-imx95-15x15-frdm:imx95-15x15-lpddr4x-frdm mc-imx95-19x19-frdm-pro:imx95-19x19-lpddr5-frdm-pro"

python() {
    add_mcdepends_with_params(d, d.getVar('IMX95EVK_MACHINE'), d.getVar('IMX95EVK_MC_MAP'))
}

imx95evk_install_examples() {
    install_examples_with_params "${IMX95EVK_MACHINE}" "${IMX95EVK_MC_MAP}"
}

ROOTFS_POSTPROCESS_COMMAND:append:imx95evk = " imx95evk_install_examples;"
