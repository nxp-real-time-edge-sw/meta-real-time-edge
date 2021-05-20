SUMMARY = "sysrepo-tsn"
DESCRIPTION = "A tool to configure TSN funtionalities in user space"
LICENSE = "Apachev2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

SRC_URI = "git://bitbucket.sw.nxp.com/dnind/sysrepo-tsn.git;protocol=ssh;nobranch=1 \
           file://sysrepo-tsnd \
           file://sysrepo-init \
           file://sysrepo-tsn.service \
           file://scripts/model-install.sh \
"

SRCREV = "76761c8ba806856a4633627a4aa51f30b2759d76"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"
RDEPENDS_${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-keystored netopeer2-server netopeer2-cli cjson libnl tsntool"

FILES_${PN} += "/etc/sysrepo-tsn"

#can modify conf/distro/include/openil-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[sysrepo-tsn-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

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
}
