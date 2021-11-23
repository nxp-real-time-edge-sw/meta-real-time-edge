SUMMARY = "GenAVB/TSN Stack"

inherit module-base systemd update-rc.d

LICENSE = "NXP-Binary-EULA & GPLv2 & BSD-3-Clause"
LIC_FILES_CHKSUM = "file://licenses/EULA.txt;md5=2acb50e7549e3925e6982a7920c26fd8 \
                    file://licenses/COPYING;md5=b234ee4d69f5fce4486a80fdaf4a4263 \
                    file://licenses/BSD-3-Clause;md5=5cc0aa6b0086f08ef02006d8a2679704 \
"

GENAVB_TSN_URL = "git://github.com/NXP/GenAVB_TSN.git;protocol=http"

SRC_URI = "${GENAVB_TSN_URL};nobranch=1 \
    file://genavb-tsn.service \
    file://libgenavb.pc \
"

SRCREV = "a866ef0b3f6c587f3b11610c0828fb859dd8aa00"

PR = "r0"

S = "${WORKDIR}/git"

DEPENDS += "libopen62541 libbpf"

PROVIDES += "libgenavb"
RPROVIDES_${PN} = "libgenavb"

INITSCRIPT_NAME = "genavb"
INITSCRIPT_PARAMS = "defaults"

GENAVB_TSN_CONFIG ?= "endpoint_tsn"
GENAVB_TSN_CONFIG_imx8mpevk = "endpoint_tsn"
GENAVB_TSN_CONFIG_ls1028ardb = "bridge"

GENAVB_TSN_TARGET ?= "linux_imx8"
GENAVB_TSN_TARGET_imx8mpevk = "linux_imx8"
GENAVB_TSN_TARGET_ls1028ardb = "linux_ls1028"

SYSTEMD_SERVICE_${PN} = "genavb-tsn.service"
SYSTEMD_AUTO_ENABLE_${PN} = "disable"

EXTRA_OEMAKE += " \
    KERNELDIR=${STAGING_KERNEL_DIR} \
    CROSS_COMPILE=${TARGET_PREFIX} \
    STAGING_DIR="${STAGING_DIR_TARGET}" \
    KBUILD_OUTPUT=${STAGING_KERNEL_BUILDDIR} \
    OPCUA_SUPPORT=1 \
"

do_compile () {
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} stack
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} apps
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} modules
}

do_install () {
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} stack-install PREFIX=${D}
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} apps-install PREFIX=${D}
	oe_runmake config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} modules-install PREFIX=${D}

	# Install the soname symlink
	ln -sf libgenavb.so.1.0 ${D}${libdir}/libgenavb.so.1

	# Install startup scripts or services
	if ${@bb.utils.contains('DISTRO_FEATURES', 'systemd', 'true', 'false', d)}; then
		install -d ${D}${systemd_system_unitdir}

		install -m0644 ${WORKDIR}/genavb-tsn.service ${D}${systemd_system_unitdir}/genavb-tsn.service

		# Remove the auto start config option as we use systemd
		sed -e '/Set following configuration to 1 to start .* automatically at boot time/,+2d' \
		    -i ${D}${sysconfdir}/genavb/config
	fi

	if ${@bb.utils.contains('DISTRO_FEATURES', 'sysvinit', 'true', 'false', d)}; then
		# Disable the stack autostart
		sed -e 's/CFG_AUTO_START=.*/CFG_AUTO_START=0/' -i ${D}${sysconfdir}/genavb/config
	else
		# Remove sysvinit installed scripts if not in distro features
		rm -rf ${D}${sysconfdir}/init.d
	fi

	# Remove unnecessary config file installed by stack-install
	rm -rf ${D}/config

	install -d ${D}${libdir}/pkgconfig

	install -m ug+rw ${WORKDIR}/libgenavb.pc ${D}${libdir}/pkgconfig/
}

# QA Issue: No GNU_HASH in the elf binary
INSANE_SKIP_${PN}-dev = "ldflags"
INSANE_SKIP_${PN} = "ldflags"
INSANE_SKIP_${PN} += "dev-so"

# Add the module and firmware directory to the package files
FILES_${PN} += "/lib/modules /lib/firmware"
