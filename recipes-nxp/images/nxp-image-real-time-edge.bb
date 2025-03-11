# Copyright 2021-2022 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

real-time-edge-IMAGE_BASE ?= "recipes-fsl/images/imx-image-multimedia.bb"
real-time-edge-IMAGE_BASE:qoriq = "recipes-fsl/images/fsl-image-networking.bb"

require ${real-time-edge-IMAGE_BASE}

SUMMARY = "Real-time Edge image to be used for development and evaluation"

LICENSE = "MIT"

export IMAGE_BASENAME = "nxp-image-real-time-edge"

IMAGE_FSTYPES:qoriq = "${SOC_DEFAULT_IMAGE_FSTYPES}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

IMAGE_INSTALL:append = " \
    packagegroup-real-time-edge-networking \
    packagegroup-real-time-edge-system \
    packagegroup-real-time-edge-industrial \
    packagegroup-real-time-edge-tools \
"

IMAGE_INSTALL:append:ls1028ardb = " \
    packagegroup-real-time-edge-multimedia \
"
IMAGE_INSTALL:append:imx-nxp-bsp = " \
    packagegroup-real-time-edge-rtos \
    packagegroup-harpoon \
"

# do_image[mcdepends] = "mc:imx8mp-lpddr4-evk:evkmimx8mp:demo-hello-world:do_install"
