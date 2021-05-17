SUMMARY = "Netopeer2 is a set of tools implementing network configuration tools based on the NETCONF Protocol."
DESCRIPTION = "Netopeer2 is based on the new generation of the NETCONF and YANG libraries - libyang and libnetconf2. The Netopeer server uses sysrepo as a NETCONF datastore implementation."
LICENSE = "BSD-3-Clause"
LIC_FILES_CHKSUM = "file://../LICENSE;md5=b7cb0021418524c05c4e5b21041d9402"

SRC_URI = "git://github.com/CESNET/Netopeer2.git;protocol=https;nobranch=1 \
	   file://0001-netopeer2-client-fix-compilation-issue.patch \
"

#PV = "0.7.12+git${SRCPV}"
SRCREV = "49281975ea78808910701b7af4cf8c7a65ae37b7"

S = "${WORKDIR}/git/cli"

DEPENDS = "libyang libnetconf2 sysrepo netopeer2-server"

inherit cmake pkgconfig

# Specify any options you want to pass to cmake using EXTRA_OECMAKE:
EXTRA_OECMAKE = " -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE:String=Release "

