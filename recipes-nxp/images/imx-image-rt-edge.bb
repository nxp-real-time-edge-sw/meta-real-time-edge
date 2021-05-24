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
IMAGE_INSTALL += " \
    imx-uuc \
    imx-test \
    packagegroup-imx-core-tools \
    packagegroup-imx-security \
    packagegroup-multimedia \
"

IMAGE_INSTALL += " \
    packagegroup-rt-edge \
"
