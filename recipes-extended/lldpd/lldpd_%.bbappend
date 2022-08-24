FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

EXTRA_OECONF += " --enable-json0 with_lldpd_ctl_socket=\"/var/run/lldpd.socket\" \
		 with_lldpd_pid_file=\"/var/run/lldpd.pid\" \
		 "

SRC_URI:append = " file://0001-lldpd-add-additional-ethernet-capabilitie-tlv.patch \
		  file://0002-lldp-deprecate-link-aggregation-TLV-from-802.3.patch"
