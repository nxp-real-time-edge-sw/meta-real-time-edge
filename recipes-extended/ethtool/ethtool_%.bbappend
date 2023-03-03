FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

SRC_URI:append = " \
	file://0001-ethtool-Update-headers-from-the-kernel.patch \
	file://0002-ethtool-add-support-for-configuring-frame-preemption.patch \
	file://0003-ethtool-add-support-for-configuring-frame-preemption.patch \
	file://0004-preemption-add-lldp-verify-parameter.patch \
	file://0005-preemption-add-disabled-parameter.patch \
"
