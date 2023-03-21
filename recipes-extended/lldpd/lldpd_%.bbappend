FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

EXTRA_OECONF += " --enable-json0 with_lldpd_ctl_socket=\"/var/run/lldpd.socket\" \
		 with_lldpd_pid_file=\"/var/run/lldpd.pid\" \
		 "

SRC_URI:append = " file://0001-lldpd-add-additional-ethernet-capabilitie-tlv.patch \
		  file://0002-lldp-deprecate-link-aggregation-TLV-from-802.3.patch \
		  file://0003-lldpd-decode-Additional-Ethernet-Capabilities.patch \
		  file://0004-lldpd-preemption-fix-status-not-updated-issue.patch \
		  file://0005-preemption-send-lldp-with-preemption-TLV-before-enab.patch \
		  file://0006-header-ethtool-update-ethtool_fp-parameters.patch \
"
