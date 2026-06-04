# i.MX943 EVK Multi-machine Examples Support

IMX943EVK_MACHINE = "imx943evk"
IMX943EVK_MC_MAP = "mc-imx943-19x19-lpddr4-evk:imx943-19x19-lpddr4-evk mc-imx943-15x15-lpddr4-evk:imx943-15x15-lpddr4-evk"

python() {
    add_mcdepends_with_params(d, d.getVar('IMX943EVK_MACHINE'), d.getVar('IMX943EVK_MC_MAP'))
}

imx943evk_install_examples() {
    install_examples_with_params "${IMX943EVK_MACHINE}" "${IMX943EVK_MC_MAP}"
}

ROOTFS_POSTPROCESS_COMMAND:append:imx943evk = " imx943evk_install_examples;"
