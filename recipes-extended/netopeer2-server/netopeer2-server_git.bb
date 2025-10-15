SUMMARY = "Netopeer2 is a set of tools implementing network configuration tools based on the NETCONF Protocol."
DESCRIPTION = "Netopeer2 is based on the new generation of the NETCONF and YANG libraries - libyang and libnetconf2. The Netopeer server uses sysrepo as a NETCONF datastore implementation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=41daedff0b24958b2eba4f9086d782e1"

SRC_URI = "git://github.com/CESNET/Netopeer2.git;protocol=https;branch=master \
          ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', \
	        'file://netopeer2-server', '', d)} \
          ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', \
	        'file://netopeer2-server.service', '', d)} \
          "

PV = "2.4.5+git"
SRCREV = "2549f8f73b61e94f031a84bf709cbb4e3d594a94"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo curl"
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}"

RDEPENDS:${PN} += "bash curl"

FILESEXTRAPATHS:prepend := "${THISDIR}/${PN}:"
FILES:${PN} += "${datadir}/yang* ${datadir}/netopeer2/* ${libdir}/sysrepo-plugind/*"

inherit cmake pkgconfig update-rc.d
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release -DINSTALL_MODULES=OFF -DGENERATE_HOSTKEY=OFF -DMERGE_LISTEN_CONFIG=OFF -DSYSREPO_SETUP=OFF"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "netopeer2-server.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

INITSCRIPT_NAME = "netopeer2-server"
INITSCRIPT_PARAMS = "start 80 5 2 3 . stop 60 0 1 6 ."

do_install:append () {
    install -d ${D}${sysconfdir}/netopeer2/scripts
    install -o root -g root ${S}/scripts/setup.sh ${D}${sysconfdir}/netopeer2/scripts/setup.sh
    install -o root -g root ${S}/scripts/merge_hostkey.sh ${D}${sysconfdir}/netopeer2/scripts/merge_hostkey.sh
    install -o root -g root ${S}/scripts/merge_config.sh ${D}${sysconfdir}/netopeer2/scripts/merge_config.sh
    install -d ${D}${sysconfdir}/netopeer2
    if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
        install -d ${D}${sysconfdir}/init.d
        install -m 0755 ${UNPACKDIR}/netopeer2-server ${D}${sysconfdir}/init.d/
    fi

    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
        install -d ${D}${systemd_system_unitdir}
        install -m 0644 ${UNPACKDIR}/netopeer2-server.service ${D}${systemd_system_unitdir}
    fi
}
