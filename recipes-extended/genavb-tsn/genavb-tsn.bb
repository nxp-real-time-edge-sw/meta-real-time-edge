SUMMARY = "GenAVB/TSN Stack binaries, scripts and apps"

require genavb-tsn.inc

inherit cmake systemd update-rc.d

SRC_URI_append = " \
    file://genavb-tsn.service \
    file://libgenavb.pc \
"

PR = "r0"

RDEPENDS_${PN} += "kernel-module-genavb-tsn"

DEPENDS += "libopen62541 libbpf"

PROVIDES += "libgenavb"
RPROVIDES_${PN} = "libgenavb"

INITSCRIPT_NAME = "genavb"
INITSCRIPT_PARAMS = "defaults"

SYSTEMD_SERVICE_${PN} = "genavb-tsn.service"
SYSTEMD_AUTO_ENABLE_${PN} = "disable"

GENAVB_TSN_DEMO_APPS = "1"

export OPCUA_SUPPORT = "1"

# Use Make instead of the default ninja generator
OECMAKE_GENERATOR = "Unix Makefiles"

EXTRA_OECMAKE += " \
    -DCMAKE_INSTALL_PREFIX="/" \
    -DTARGET=${GENAVB_TSN_TARGET} \
    -DCONFIG=${GENAVB_TSN_CONFIG} \
    -DCROSS_COMPILE=${TARGET_PREFIX} \
    -DSTAGING_DIR="${STAGING_DIR_TARGET}" \
    -DBUILD_KERNEL_MODULE=OFF \
    -DBUILD_APPS=${@bb.utils.contains('GENAVB_TSN_DEMO_APPS', '1', 'ON', 'OFF', d)} \
"

do_install_append () {
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

	install -d ${D}${libdir}/pkgconfig

	install -m ug+rw ${WORKDIR}/libgenavb.pc ${D}${libdir}/pkgconfig/
}

# QA Issue: No GNU_HASH in the elf binary
INSANE_SKIP_${PN}-dev = "ldflags"
INSANE_SKIP_${PN} = "ldflags"
INSANE_SKIP_${PN} += "dev-so"

# Add the firmware directory to the package files
FILES_${PN} += "/lib/firmware"
