# Recipe created by recipetool
SUMMARY = "YANG-based configuration and operational state data store for Unix/Linux applications."
DESCRIPTION = ""
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://LICENSE;md5=ef345f161efb68c3836e6f5648b2312f"

SRC_URI = "git://github.com/sysrepo/sysrepo.git;protocol=https;branch=master \
           file://0001-correct-path-to-tar-binary.patch \
           ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', \
                'file://sysrepo-plugind','', d)} \
           ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', \
                'file://sysrepo-plugind.service','', d)}"

PV = "3.7.11+git"
SRCREV = "1b720b196f630f348d9e0c131d326b3fb8c6aca7"

S = "${WORKDIR}/git"

DEPENDS = "libyang protobuf protobuf-c protobuf-c-native libredblack libev libnetconf2"
DEPENDS += "${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}"

FILES:${PN} += "${datadir}/yang/* ${libdir}/sysrepo-plugind/*"

inherit cmake pkgconfig python3native python3-dir update-rc.d
inherit ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'systemd', '', d)}

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX:PATH=/usr -DCMAKE_BUILD_TYPE:String=Release -DBUILD_EXAMPLES:String=False -DENABLE_TESTS:String=False -DREPOSITORY_LOC:PATH=/etc/sysrepo -DCALL_TARGET_BINS_DIRECTLY=False -DGEN_LANGUAGE_BINDINGS:String=False "

BBCLASSEXTEND = "native nativesdk"

SYSTEMD_PACKAGES = "${PN}"
SYSTEMD_SERVICE:${PN} = "sysrepo-plugind.service"
SYSTEMD_AUTO_ENABLE:${PN} = "enable"

INITSCRIPT_NAME = "sysrepo-plugind"
INITSCRIPT_PARAMS = "start 60 5 2 3 . stop 80 0 1 6 ."

RDEPENDS:${PN} += "tar"

do_install:append () {
    install -d ${D}${sysconfdir}/sysrepo/data/notifications
    install -d ${D}${sysconfdir}/sysrepo/yang
    install -o root -g root ${S}/modules/ietf-netconf-notifications@2012-02-06.yang ${D}${sysconfdir}/sysrepo/yang/
    install -o root -g root ${S}/modules/ietf-netconf-with-defaults@2011-06-01.yang ${D}${sysconfdir}/sysrepo/yang/
    install -o root -g root ${S}/modules/ietf-netconf@2013-09-29.yang ${D}${sysconfdir}/sysrepo/yang/

    install -d ${D}${sysconfdir}/init.d
    if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
        install -m 0755 ${UNPACKDIR}/sysrepo-plugind ${D}${sysconfdir}/init.d/
        install -d ${D}${libdir}/sysrepo/plugins
    fi

    if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
        install -d ${D}${systemd_system_unitdir}
        install -m 0644 ${UNPACKDIR}/sysrepo-plugind.service ${D}${systemd_system_unitdir}
    fi
}
