SUMMARY = "rt-edge-sysrepo"
DESCRIPTION = "A tool to configure TSN funtionalities in user space"
LICENSE = "Apachev2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

SRC_URI = "git://bitbucket.sw.nxp.com/dnind/rt-edge-sysrepo.git;protocol=ssh;nobranch=1 \
           file://sysrepo-tsnd \
           file://sysrepo-init \
           file://sysrepo-tsn.service \
           file://sysrepo-cfg.service \
           file://scripts/model-install.sh \
           file://scripts/sysrepo-cfg.sh \
"

SRCREV = "8064c0efaa56e2d4a855885713271f043bfca0ee"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"
RDEPENDS_${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"

FILES_${PN} += "/etc/sysrepo-tsn /lib/systemd/system/* /etc/systemd/system/multi-user.target.wants/*"

#can modify conf/distro/include/rt-edge-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[rt-edge-sysrepo-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

inherit cmake pkgconfig
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release -DSYSREPOCTL_EXECUTABLE=/usr/bin/sysrepoctl -DSYSREPOCFG_EXECUTABLE=/usr/bin/sysrepocfg "

do_install_append () {
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
    ln -sfr ${D}/etc/init.d/sysrepo-tsnd ${D}/etc/rc5.d/S52sysrepo-tsn
    ln -sfr ${D}/etc/init.d/sysrepo-tsnd ${D}/etc/rc6.d/K51sysrepo-tsn
    ln -sfr ${D}/etc/init.d/sysrepo-init ${D}/etc/rc5.d/S40sysrepo-init

    install -d ${D}/lib/systemd/system/
    install -m 0775 ${WORKDIR}/sysrepo-cfg.service ${D}/lib/systemd/system/

    install -d ${D}/etc/systemd/system/multi-user.target.wants/
    ln -sfr ${D}/lib/systemd/system/sysrepo-cfg.service ${D}/etc/systemd/system/multi-user.target.wants/sysrepo-cfg.service
}
