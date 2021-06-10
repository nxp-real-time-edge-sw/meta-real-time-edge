# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

RT-EDGE-IMAGE_BASE_imx = "recipes-fsl/images/imx-image-core.bb"
RT-EDGE-IMAGE_BASE_qoriq = "recipes-fsl/images/fsl-image-networking.bb"

require ${RT-EDGE-IMAGE_BASE}

SUMMARY = "RT Edge image to be used for development and evaluation"

LICENSE = "MIT"

export IMAGE_BASENAME = "nxp-image-rt-edge"

IMAGE_FSTYPES_qoriq = "${SOC_DEFAULT_IMAGE_FSTYPES}"

PACKAGE_ARCH = "${MACHINE_ARCH}"

IMAGE_INSTALL_append = " \
    packagegroup-rt-edge-networking \
    packagegroup-rt-edge-system \
    packagegroup-rt-edge-industrial \
    packagegroup-rt-edge-tools \
"

IMAGE_INSTALL_append_ls1028ardb = " \
    packagegroup-rt-edge-multimedia \
"
IMAGE_INSTALL_append_imx = " \
    packagegroup-rt-edge-multimedia \
"