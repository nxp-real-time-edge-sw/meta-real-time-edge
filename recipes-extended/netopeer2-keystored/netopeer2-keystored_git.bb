SUMMARY = "Netopeer2 is a set of tools implementing network configuration tools based on the NETCONF Protocol."
DESCRIPTION = "Netopeer2 is based on the new generation of the NETCONF and YANG libraries - libyang and libnetconf2. The Netopeer server uses sysrepo as a NETCONF datastore implementation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://../LICENSE;md5=b7cb0021418524c05c4e5b21041d9402"

SRC_URI = "git://github.com/CESNET/Netopeer2.git;protocol=https;nobranch=1 \
           file://netopeer2-keystored \
"

# Modify these as desired
#PV = "0.7.12+git${SRCPV}"
SRCREV = "49281975ea78808910701b7af4cf8c7a65ae37b7"

S = "${WORKDIR}/git/keystored"

DEPENDS = "libyang libnetconf2 sysrepo coreutils openssh openssl"

FILES:${PN} += "/usr/lib/sysrepo/* /usr/share/*"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release -DSYSREPOCTL_EXECUTABLE=/usr/bin/sysrepoctl -DSYSREPOCFG_EXECUTABLE=/usr/bin/sysrepocfg -DCHMOD_EXECUTABLE=/bin/chmod -DSSH_KEY_INSTALL=OFF -DSYSREPOCTL_ROOT_PERMS='-p 666' "

do_install () {
    install -d ${D}/usr/share/netopeer2-keystored
    cp -r ${S}/stock_key_config.xml ${D}/usr/share/netopeer2-keystored/
    install -d ${D}/etc/sysrepo/yang
    cp -r ${S}/../modules/ietf-keystore.yang ${D}/etc/sysrepo/yang/
    install -d ${D}/usr/lib/sysrepo/plugins
    install -m 0755 ${WORKDIR}/build/libkeystored.so ${D}/usr/lib/sysrepo/plugins/

#    install -d ${D}/etc/init.d
#    install -m 0755 ${WORKDIR}/netopeer2-keystored ${D}/etc/init.d/
}
