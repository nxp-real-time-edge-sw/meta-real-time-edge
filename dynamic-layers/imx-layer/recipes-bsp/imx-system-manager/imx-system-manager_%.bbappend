FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
    file://0002-devices-MIMX95-Disable-DDR-Auto-Clock-Gating.patch \
"
