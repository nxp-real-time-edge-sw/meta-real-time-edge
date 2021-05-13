SUMMARY = "Netopeer2 is a set of tools implementing network configuration tools based on the NETCONF Protocol."
DESCRIPTION = "Netopeer2 is based on the new generation of the NETCONF and YANG libraries - libyang and libnetconf2. The Netopeer server uses sysrepo as a NETCONF datastore implementation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://../LICENSE;md5=b7cb0021418524c05c4e5b21041d9402"

SRC_URI = "git://github.com/CESNET/Netopeer2.git;protocol=https;nobranch=1 \
           file://scripts/libyang-model-install.sh \
           file://scripts/server-model-install.sh \
           file://scripts/netopeer2-keystored.sh \
           file://scripts/keystored-model-install.sh \
           file://scripts/stock_config.xml \
           file://scripts/stock_key_config.xml \
           file://0001-netopeer2-server-fix-compile-issue.patch \
           file://0002-change-PIDFILE_PREFIX-to-tmp.patch \
           file://0003-don-t-checkout-sysrepoctl-and-sysrepocfg.patch \
           file://netopeer2-server \
"

#PV = "0.7.12+git${SRCPV}"
SRCREV = "49281975ea78808910701b7af4cf8c7a65ae37b7"

S = "${WORKDIR}/git/server"
B = "${WORKDIR}/git"

DEPENDS = "libyang libnetconf2 sysrepo curl"
RDEPENDS_${PN} += "bash curl"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DKEYSTORED_KEYS_DIR=/etc/keystored/keys -DSYSREPOCTL_ROOT_PERMS='-p 666' "

do_install_append () {
    install -d ${D}/usr/share/netopeer2-server
    install -d ${D}/etc/sysrepo/yang
    install -d ${D}/etc/Netopeer2/modules
    install -o root -g root ${S}/../modules/*.yang ${D}/etc/Netopeer2/modules/

    install -d ${D}/etc/Netopeer2/scripts
    install -o root -g root ${S}/../../scripts/*.sh ${D}/etc/Netopeer2/scripts/
    install -o root -g root ${S}/../../scripts/*.xml ${D}/etc/Netopeer2/scripts/

    install -d ${D}/etc/netopeer2
    install -d ${D}/etc/init.d
    install -m 0755 ${WORKDIR}/netopeer2-server ${D}/etc/init.d/

    install -d ${D}/etc/rc5.d
    install -d ${D}/etc/rc6.d
    ln -sfr ${D}/etc/init.d/netopeer2-server ${D}/etc/rc5.d/S91netopeer2-server
    ln -sfr ${D}/etc/init.d/netopeer2-server ${D}/etc/rc6.d/K50netopeer2-server
}
