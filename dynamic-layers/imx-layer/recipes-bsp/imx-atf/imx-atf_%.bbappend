FILESEXTRAPATHS:prepend := "${THISDIR}/files:"
SRC_URI:append:real-time-edge = " \
     ${@bb.utils.contains('DISTRO_FEATURES', 'baremetal',  \
    'file://0001-Baremetal-make-UART4-accessed-by-A53-cores.patch', \
    '', d)} \
"
SRC_URI:append = " \
    file://0001-feat-gicv3-add-APIs-for-clearing-pending-interrupts.patch \
    file://0002-fix-bl31-add-workaround-for-irq-losing-in-case-of-pe.patch \
    file://0003-feat-imx93-add-common-interrupt-handler-and-enable-i.patch \
    file://0004-feat-imx-add-SIP-cpu_off-service-for-imx93.patch \
    file://0005-plat-imx93-change-dram-dvfs-to-use-intercore-service.patch \
    file://0006-plat-imx8m-change-dram-dvfs-to-use-intercore-service.patch \
    file://0007-plat-imx8m-add-SIP-cpu_off-service.patch \
    file://0008-plat-imx91-change-to-use-common-interrupt-service.patch \
    file://0009-plat-imx9-add-SIP-cpu_off-service.patch \
    file://0010-imx8mp-Set-RDC-to-level-0.patch \
    file://0011-plat-imx93-add-platform-API-plat_core_mpidr_by_idx.patch \
    file://0012-bl31-add-multicore-fastboot-support.patch \
    file://0013-plat-imx93-enable-fastboot-support.patch \
"
