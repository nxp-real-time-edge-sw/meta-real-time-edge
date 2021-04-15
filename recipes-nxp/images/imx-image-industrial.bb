# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

DESCRIPTION = "This is the basic industrial core image"

inherit core-image

LICENSE = "MIT"

export IMAGE_BASENAME = "imx-image-industrial"

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
    ${CLINFO} \
"
IMAGE_INSTALL += " \
    packagegroup-industrial-app \
"
