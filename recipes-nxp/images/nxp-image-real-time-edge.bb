# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

real-time-edge-IMAGE_BASE_imx = "recipes-fsl/images/imx-image-core.bb"
real-time-edge-IMAGE_BASE_qoriq = "recipes-fsl/images/fsl-image-networking.bb"

require ${real-time-edge-IMAGE_BASE}

SUMMARY = "Real-time Edge image to be used for development and evaluation"

LICENSE = "MIT"

export IMAGE_BASENAME = "nxp-image-real-time-edge"

IMAGE_FSTYPES_qoriq = "${SOC_DEFAULT_IMAGE_FSTYPES}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

IMAGE_INSTALL_append = " \
    packagegroup-real-time-edge-networking \
    packagegroup-real-time-edge-system \
    packagegroup-real-time-edge-industrial \
    packagegroup-real-time-edge-tools \
"

IMAGE_INSTALL_append_ls1028ardb = " \
    packagegroup-real-time-edge-multimedia \
"
IMAGE_INSTALL_append_imx = " \
    packagegroup-real-time-edge-multimedia \
"