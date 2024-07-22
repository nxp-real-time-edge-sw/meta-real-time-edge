FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI += "file://deferred.sh"

do_install:append() {
# Create directories and install device independent scripts
#
	install -d ${D}${sysconfdir}/init.d
	install -m 0755    ${WORKDIR}/deferred.sh       ${D}${sysconfdir}/init.d
	update-rc.d -r ${D} deferred.sh start 20 2 3 4 5 .
}
