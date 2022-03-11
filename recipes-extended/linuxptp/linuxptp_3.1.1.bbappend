
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append = " \
    file://0001-Add-IEEE-802.1AS-2011-time-aware-bridge-support.patch \
    file://0002-Add-BMCA-support-for-IEEE-802.1AS-2011.patch \
    file://0003-port-drop-erroneous-neighbor-rate-ratio.patch \
    file://0004-clock-redecide-state-if-get-EV_FAULT_DETECTED-event.patch \
    file://0005-port-switch-PHC-in-jbod-mode-when-in-UNCALIBRATED-or.patch \
    file://0006-phc2sys-break-long-lines-in-the-PTP-management-messa.patch \
    file://0007-phc2sys-extract-PMC-functionality-into-a-smaller-str.patch \
    file://0008-phc2sys-make-PMC-functions-non-static.patch \
    file://0009-phc2sys-break-out-pmc-code-into-pmc_common.c.patch \
    file://0010-ts2phc-create-a-private-data-structure.patch \
    file://0011-ts2phc-instantiate-a-full-clock-structure-for-every-.patch \
    file://0012-ts2phc-instantiate-a-full-clock-structure-for-every-.patch \
    file://0013-ts2phc-instantiate-a-pmc-node.patch \
    file://0014-ts2phc_slave-print-master-offset.patch \
    file://0015-ts2phc-split-slave-poll-from-servo-loop.patch \
    file://0016-ts2phc-reconfigure-sync-direction-by-subscribing-to-.patch \
    file://0017-ts2phc-allow-the-PHC-PPS-master-to-be-synchronized.patch \
    file://0018-ts2phc_phc_master-make-use-of-new-kernel-API-for-per.patch \
    file://0019-README-mention-support-of-IEEE-802.1AS-time-aware-br.patch \
    file://0020-Revert-port-drop-erroneous-neighbor-rate-ratio.patch \
    file://0021-port-Fix-link-down-up-to-continue-using-phc_index-se.patch \
    file://0022-configs-increase-tx_timestamp_timeout-for-default-gP.patch \
    file://0023-configs-use-neighborPropDelayThresh-default-value-fo.patch \
    file://0024-ts2phc_slave-fix-memory-leak-in-ts2phc_slave_array_c.patch \
"

do_install:append () {
    install -d ${D}/etc/ptp4l_cfg
    install ${S}/configs/* ${D}/etc/ptp4l_cfg

    install -p ${S}/phc_ctl  ${D}/${bindir}
}
