# i.MX8M Plus EVK Multi-machine Examples Support

IMX8MPEVK_MACHINE = "imx8mpevk"
IMX8MPEVK_MC_MAP = "mc-imx8mp-navq:imx8mp-navq mc-imx8mp-ddr4-evk:imx8mp-ddr4-evk mc-imx8mp-lpddr4-frdm:imx8mp-lpddr4-frdm"

python() {
    add_mcdepends_with_params(d, d.getVar('IMX8MPEVK_MACHINE'), d.getVar('IMX8MPEVK_MC_MAP'))
}

imx8mpevk_install_examples() {
    install_examples_with_params "${IMX8MPEVK_MACHINE}" "${IMX8MPEVK_MC_MAP}"
}

ROOTFS_POSTPROCESS_COMMAND:append:imx8mpevk = " imx8mpevk_install_examples;"
