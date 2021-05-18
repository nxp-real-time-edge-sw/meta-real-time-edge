# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "This is the basic rt-edge core image"

inherit core-image

LICENSE = "MIT"

export IMAGE_BASENAME = "imx-image-rt-edge"

IMAGE_FEATURES += " \
    debug-tweaks \
    tools-profile \
    tools-sdk \
    package-management \
    splash \
    nfs-server \
    tools-debug \
    ssh-server-dropbear \
    hwcodecs \
"
SDKIMAGE_FEATURES_append = " \
    staticdev-pkgs \
"
CLINFO ?= ""
CLINFO_imxgpu = "clinfo"
CLINFO_mx8mm = ""

IMAGE_INSTALL += " \
    ${@bb.utils.contains('DISTRO_FEATURES', 'wayland', ' weston weston-examples weston-init','', d)} \
    imx-uuc \
    imx-test \
    packagegroup-imx-core-tools \
    packagegroup-imx-security \
    libopencl-imx \
    imx-gpu-viv-demos \
    packagegroup-fsl-tools-gpu \
    packagegroup-imx-tools-audio \
    weston-init weston-examples gtk+3-demo \
    packagegroup-fsl-gstreamer1.0-full \
    ${CLINFO} \
"

IMAGE_INSTALL += " \
    packagegroup-rt-edge \
"
