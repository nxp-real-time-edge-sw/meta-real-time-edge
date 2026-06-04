# i.MX91 EVK Multi-machine Examples Support

IMX91EVK_MACHINE = "imx91evk"
IMX91EVK_MC_MAP = "mc-imx91-9x9-qsb:imx91-9x9-lpddr4-qsb mc-imx91-11x11-frdm:imx91-11x11-lpddr4-frdm mc-imx91-11x11-frdm-imx91s:imx91-11x11-lpddr4-frdm-imx91s"

python() {
    add_mcdepends_with_params(d, d.getVar('IMX91EVK_MACHINE'), d.getVar('IMX91EVK_MC_MAP'))
}

imx91evk_install_examples() {
    install_examples_with_params "${IMX91EVK_MACHINE}" "${IMX91EVK_MC_MAP}"
}

ROOTFS_POSTPROCESS_COMMAND:append:imx91evk = " imx91evk_install_examples;"
