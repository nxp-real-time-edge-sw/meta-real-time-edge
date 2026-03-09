SUMMARY = "real-time-edge-sysrepo"
DESCRIPTION = "A tool to configure TSN functionalities in user space"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

REAL_TIME_EDGE_SYSREPO_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-sysrepo.git;protocol=https"
REAL_TIME_EDGE_SYSREPO_BRANCH ?= "master"
REAL_TIME_EDGE_SYSREPO_SRCREV ?= "721441283c3cf26046ebaf962356809e41916153"

SRC_URI = "${REAL_TIME_EDGE_SYSREPO_SRC};branch=${REAL_TIME_EDGE_SYSREPO_BRANCH}"
SRC_URI += "file://sysrepo-plugins-install.sh"
SRC_URI += "file://sysrepo-plugins-list.conf"

SRCREV = "${REAL_TIME_EDGE_SYSREPO_SRCREV}"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool"
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'genavb-tsn', 'genavb-tsn', '', d)}"

RDEPENDS:${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool lshw"
RDEPENDS:${PN} += "${@bb.utils.contains('DISTRO_FEATURES', 'genavb-tsn', 'genavb-tsn', '', d)}"

FILES:${PN} += "${datadir}/yang/* ${libdir}/sysrepo-plugind/* ${libdir}/sysrepo-plugind/staging"

# can modify conf/distro/include/real-time-edge-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[real-time-edge-sysrepo-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

inherit cmake pkgconfig

EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "

do_install:append () {
    install -m 0755 -D ${UNPACKDIR}/sysrepo-plugins-install.sh ${D}${bindir}/sysrepo-plugins-install.sh
    install -m 0644 -D ${UNPACKDIR}/sysrepo-plugins-list.conf ${D}${sysconfdir}/sysrepo-plugins-list.conf
}

