# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)
DESCRIPTION = "Package group used by NXP Community to provide graphic packages used \
to test the several hardware accelerated graphics APIs including packages not \
provided by NXP."
SUMMARY = "NXP multimedia group"

PACKAGE_ARCH = "${MACHINE_ARCH}"

inherit packagegroup

SOC_GPU= ""
SOC_GPU_imxgpu = "libopencl-imx imx-gpu-viv-demos gtk+3-demo \
                  packagegroup-fsl-tools-gpu packagegroup-imx-tools-audio"
SOC_GPU_ls1028ardb = "libopencl-imx imx-gpu-viv-demos gtk+3-demo \
                  packagegroup-fsl-tools-gpu packagegroup-imx-tools-audio"

RDEPENDS_${PN} = " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', ' weston weston-examples weston-init','', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'libdrm', 'libdrm', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'imx-gpu-viv', 'imx-gpu-viv', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland-protocols', 'wayland-protocols', '', d)} \
    ${@bb.utils.contains('DISTRO_FEATURES', 'kmscube', 'kmscube', '', d)} \
    ${SOC_GPU} \
    packagegroup-fsl-gstreamer1.0-full \
"
