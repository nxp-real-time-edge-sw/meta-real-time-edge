SUMMARY = "A small image just capable of allowing a device to boot."

IMAGE_INSTALL = "packagegroup-core-boot ${CORE_IMAGE_EXTRA_INSTALL}"

IMAGE_LINGUAS = " "

LICENSE = "MIT"

inherit core-image

PACKAGE_ARCH = "${MACHINE_ARCH}"

export IMAGE_BASENAME = "nxp-image-real-time-edge-plc"

IMAGE_ROOTFS_SIZE ?= "8192"
IMAGE_ROOTFS_EXTRA_SPACE:append = "${@bb.utils.contains("DISTRO_FEATURES", "systemd", " + 4096", "", d)}"

IMAGE_FEATURES += "ssh-server-dropbear"

IMAGE_INSTALL:append = " \
    net-tools \
    kernel-modules \
    stress-ng \
    rt-tests \
"

MACHINE_FEATURES:remove = " \
    optee \
"

