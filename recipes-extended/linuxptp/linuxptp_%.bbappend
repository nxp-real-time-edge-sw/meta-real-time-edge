
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append = " \
    file://0001-Add-IEEE-802.1AS-2011-time-aware-bridge-support.patch \
    file://0002-Add-BMCA-support-for-IEEE-802.1AS-2011.patch \
    file://0003-clock-redecide-state-if-get-EV_FAULT_DETECTED-event.patch \
    file://0004-port-switch-PHC-in-jbod-mode-when-in-UNCALIBRATED-or.patch \
    file://0005-README-mention-support-of-IEEE-802.1AS-time-aware-br.patch \
    file://0006-configs-increase-tx_timestamp_timeout-for-default-gP.patch \
    file://0007-onfigs-use-neighborPropDelayThresh-default-value-for.patch \
    file://0008-vclock-use-missing.h-to-instead-linux-header.patch \
"

do_install:append () {
    install -d ${D}/etc/ptp4l_cfg
    install ${S}/configs/* ${D}/etc/ptp4l_cfg

    install -d ${D}${bindir}
    install -p ${S}/phc_ctl  ${D}${bindir}
}

FILES:${PN} += "${bindir}"
