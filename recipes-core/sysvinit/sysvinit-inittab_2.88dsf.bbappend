# Freescale imx extra configuration
RDEPENDS:${PN} += " bash "

do_install:append:imxgpu2d() {

	# Uninstall rc_mxc.S to /etc/init.d
	sed -i '/rc_mxc/d' ${D}${sysconfdir}/inittab
}

