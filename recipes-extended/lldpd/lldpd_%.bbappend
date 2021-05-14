FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

S = "${WORKDIR}/git"
B = "${S}"

FILES_${PN} += "/usr/etc/* /usr/etc/lldpd.d/* /usr/share/* ${sbindir}/* ${libdir}/*"

SRC_URI_rt-edge = "git://github.com/lldpd/lldpd.git;protocol=https;branch=master; \
			file://lldpd.init.d \
			file://lldpd.default \
		      "
SRCREV_rt-edge = "b877664dec7ef854f7f7f893b9c8906aaff8f369"

EXTRA_OECONF += "--prefix=/usr/ --host=${TARGET_SYS} --build=${BUILD_SYS}"

do_configure() {
    ${S}/autogen.sh
    ${S}/configure ${EXTRA_OECONF} 
}
