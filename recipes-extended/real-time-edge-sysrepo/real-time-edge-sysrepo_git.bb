SUMMARY = "real-time-edge-sysrepo"
DESCRIPTION = "A tool to configure TSN functionalities in user space"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

REAL_TIME_EDGE_SYSREPO_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-sysrepo.git;protocol=https;nobranch=1"

SRC_URI = "${REAL_TIME_EDGE_SYSREPO_SRC}"

SRC_URI += " \
    file://sysrepo-tsnd \
    file://sysrepo-init \
    file://scripts/model-install.sh \
"

SRCREV = "1f254c5760a3896e4b02090e75d4eb7a634c2393"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"
RDEPENDS:${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"

FILES:${PN} += "/etc/sysrepo-tsn"

# can modify conf/distro/include/real-time-edge-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[real-time-edge-sysrepo-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

inherit cmake pkgconfig

EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release -DSYSREPOCTL_EXECUTABLE=/usr/bin/sysrepoctl -DSYSREPOCFG_EXECUTABLE=/usr/bin/sysrepocfg "

do_install:append () {
    install -d ${D}/etc/sysrepo-tsn/
    install -d ${D}/etc/sysrepo-tsn/scripts
    install -m 0775 ${WORKDIR}/scripts/*.sh ${D}/etc/sysrepo-tsn/scripts
    install -d ${D}/etc/sysrepo-tsn/modules
    install -m 0775 ${S}/modules/*.yang ${D}/etc/sysrepo-tsn/modules

    install -d ${D}/etc/init.d
    install -m 0755 ${WORKDIR}/sysrepo-tsnd ${D}/etc/init.d/
    install -m 0755 ${WORKDIR}/sysrepo-init ${D}/etc/init.d/

    install -d ${D}/etc/rc5.d
    install -d ${D}/etc/rc6.d
    ln -sfr ${D}/etc/init.d/sysrepo-tsnd ${D}/etc/rc5.d/S52sysrepo-tsnd
    ln -sfr ${D}/etc/init.d/sysrepo-tsnd ${D}/etc/rc6.d/K51sysrepo-tsnd
    ln -sfr ${D}/etc/init.d/sysrepo-init ${D}/etc/rc5.d/S40sysrepo-init
}
