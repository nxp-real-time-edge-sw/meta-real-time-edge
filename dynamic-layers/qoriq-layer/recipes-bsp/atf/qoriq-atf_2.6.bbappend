FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
    file://0001-GIC-V2-remove-the-configuration-for-SGI-8-15.patch \
"
