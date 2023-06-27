FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"

SRC_URI += "file://profile"

do_install:append:imx-nxp-bsp () {
    install -Dm0755 ${WORKDIR}/profile ${D}${sysconfdir}/profile.d/weston.sh
}
