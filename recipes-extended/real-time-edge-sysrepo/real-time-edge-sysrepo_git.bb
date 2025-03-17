SUMMARY = "real-time-edge-sysrepo"
DESCRIPTION = "A tool to configure TSN functionalities in user space"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

REAL_TIME_EDGE_SYSREPO_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-sysrepo.git;protocol=https"
REAL_TIME_EDGE_SYSREPO_BRANCH ?= "master"
REAL_TIME_EDGE_SYSREPO_SRCREV ?= "a6119642c3f224b270228d1f9781f03f4b6cb235"

SRC_URI = "${REAL_TIME_EDGE_SYSREPO_SRC};branch=${REAL_TIME_EDGE_SYSREPO_BRANCH} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'file://sysrepo-tsn', '', d)}"

SRCREV = "${REAL_TIME_EDGE_SYSREPO_SRCREV}"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool"
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}"

RDEPENDS:${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool"

FILES:${PN} += "${datadir}/yang/*"

# can modify conf/distro/include/real-time-edge-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[real-time-edge-sysrepo-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

inherit cmake pkgconfig update-rc.d
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}

EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "sysrepo-tsn.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

INITSCRIPT_NAME = "sysrepo-tsn"
INITSCRIPT_PARAMS = "start 70 5 2 3 . stop 70 0 1 6 ."

do_install:append () {
    if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
        install -d ${D}${sysconfdir}/init.d
        install -m 0755 ${WORKDIR}/sysrepo-tsn ${D}${sysconfdir}/init.d/
    fi
}

