FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
     ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    'file://0001-Baremetal-make-UART4-accessed-by-A53-cores.patch', \
    '', d)} \
"
SRC_URI:append = " \
    file://0001-plat-imx93-refine-the-EL3-interrupt-handle.patch \
    file://0002-plat-imx-add-inter-core-service-common-driver.patch \
    file://0003-plat-imx93-change-dram-dvfs-to-use-intercore-service.patch \
    file://0004-drivers-gicv3-add-APIS-for-clearing-pending-interrup.patch \
    file://0005-plat-imx-add-SIP-cpu_off-service-for-imx93.patch \
    file://0006-plat-imx8m-change-dram-dvfs-to-use-intercore-service.patch \
    file://0007-plat-imx8m-add-SIP-cpu_off-service.patch \
    file://0008-plat-imx91-change-to-use-common-interrupt-service.patch \
    file://0009-plat-imx95-add-SIP-cpu_off-service.patch \
    file://0001-bl31-add-workaround-for-irq-losing-in-case-of-pendin.patch \
    file://0002-plat-imx8m-imx93-5-enable-workaround-for-CPU-off-SIP.patch \
"
