# Copyright 2021 NXP
# Released under the MIT license (see COPYING.MIT for the terms)

require recipes-core/images/core-image-minimal.bb

SUMMARY = "OpenIL image to be used for development and evaluation"
DESCRIPTION = "OpenIL image which includes all the tested tools and \
Freescale-specific packages. It is a full Linux system rather than \
an embedded system for development and evaluation tasks."

LICENSE = "MIT"

PACKAGE_ARCH = "${MACHINE_ARCH}"

CORE_IMAGE_EXTRA_INSTALL += "udev-extraconf lsb-release"
CORE_IMAGE_EXTRA_INSTALL_append_qoriq = " udev-rules-qoriq"

# copy the manifest and the license text for each package to image
COPY_LIC_MANIFEST = "1"


IMAGE_INSTALL_append = " \
    packagegroup-core-ssh-openssh \
    packagegroup-fsl-mfgtools \
    packagegroup-fsl-tools-core \
    packagegroup-fsl-benchmark-core \
    packagegroup-fsl-networking-core \
    packagegroup-rt-edge \
"

IMAGE_INSTALL_append_ls1028ardb += " \
    libopencl-imx \
    imx-gpu-viv-demos \
    packagegroup-fsl-tools-gpu \
    packagegroup-imx-tools-audio \
    weston-init weston-examples gtk+3-demo \
    packagegroup-fsl-gstreamer1.0-full \
"
