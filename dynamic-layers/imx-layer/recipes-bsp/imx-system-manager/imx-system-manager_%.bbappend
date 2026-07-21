FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
    file://0001-devices-MIMX95-Disable-DDR-Auto-Clock-Gating.patch \
    file://0002-configs-imx95rte-update-for-booting-M7-from-u-boot.patch \
    file://0003-configs-imx943-add-config-for-Real-Time-Edge-cases.patch \
    file://0004-configs-mx94rte-assign-access-permission-of-MSGINTR2.patch \
    file://0005-configs-mx94rte-add-access-to-lpuart12-for-A55.patch \
    file://0006-configs-mx94rte-disable-auto-clock-gating.patch \
    file://0007-configs-mx94rte-update-for-booting-M-cores-from-u-bo.patch \
    file://0008-configs-mx952rte-copy-from-configs-mx952evk.cfg.patch \
    file://0009-configs-mx952rte-add-i.mx952-support-for-real-time-e.patch \
    file://0010-configs-mx952rte-enable-ca55-to-control-cm7.patch \
"
