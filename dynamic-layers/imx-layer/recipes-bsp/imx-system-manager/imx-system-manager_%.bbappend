FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
    file://0001-devices-MIMX95-Disable-DDR-Auto-Clock-Gating.patch \
    file://0002-configs-imx95rte-update-for-booting-M7-from-u-boot.patch \
"
