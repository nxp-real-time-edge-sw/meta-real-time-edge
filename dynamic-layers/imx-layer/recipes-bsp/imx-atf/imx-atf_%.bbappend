FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
     ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    'file://0001-Baremetal-make-UART4-accessed-by-A53-cores.patch', \
    '', d)} \
"
