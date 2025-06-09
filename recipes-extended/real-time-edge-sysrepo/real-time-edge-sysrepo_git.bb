SUMMARY = "real-time-edge-sysrepo"
DESCRIPTION = "A tool to configure TSN functionalities in user space"
LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=86d3f3a95c324c9479bd8986968f4327"

REAL_TIME_EDGE_SYSREPO_SRC ?= "git://github.com/nxp-real-time-edge-sw/real-time-edge-sysrepo.git;protocol=https"
REAL_TIME_EDGE_SYSREPO_BRANCH ?= "master"
REAL_TIME_EDGE_SYSREPO_SRCREV ?= "1bb827dbc3bbe88b950b81c29571e25b3394c17a"

SRC_URI = "${REAL_TIME_EDGE_SYSREPO_SRC};branch=${REAL_TIME_EDGE_SYSREPO_BRANCH}"

SRCREV = "${REAL_TIME_EDGE_SYSREPO_SRCREV}"

S = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool"

RDEPENDS:${PN} += "bash curl libyang libnetconf2 sysrepo netopeer2-server cjson libnl tsntool"

FILES:${PN} += "${datadir}/yang/* ${libdir}/sysrepo-plugind/*"

# can modify conf/distro/include/real-time-edge-base.inc for PACKAGECONFIG
PACKAGECONFIG ??= ""
PACKAGECONFIG[real-time-edge-sysrepo-tc] = "-DCONF_SYSREPO_TSN_TC=ON,-DCONF_SYSREPO_TSN_TC=OFF,"

inherit cmake pkgconfig

EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "
