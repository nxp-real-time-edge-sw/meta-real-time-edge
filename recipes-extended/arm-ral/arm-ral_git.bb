SUMMARY = "Netopeer2 is a set of tools implementing network configuration tools based on the NETCONF Protocol."
DESCRIPTION = "Netopeer2 is based on the new generation of the NETCONF and YANG libraries - libyang and libnetconf2. The Netopeer server uses sysrepo as a NETCONF datastore implementation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://../LICENSE;md5=b7cb0021418524c05c4e5b21041d9402"

SRC_URI = "git://gitlab.arm.com/networking/ral.git;protocol=https;nobranch=1 \
"

SRCREV = "d9db4495c58f372286abd86c5bb56a5c1f0b2732"

S = "${WORKDIR}/git"

#DEPENDS = "sysrepo"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DBUILD_TESTING=On -DBUILD_EXAMPLES=On -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "

#do_install () {
#    install -d ${D}/etc/init.d
#    install -m 0755 ${WORKDIR}/netopeer2-keystored ${D}/etc/init.d/
#}
