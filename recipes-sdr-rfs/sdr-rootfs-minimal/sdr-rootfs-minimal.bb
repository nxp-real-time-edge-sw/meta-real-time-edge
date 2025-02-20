require ${BSPDIR}/sources/poky/meta/recipes-core/images/core-image-minimal.bb

DESCRIPTION = "A small image just capable of allowing a device to boot and \
is suitable for development work."

IMAGE_FEATURES += "dev-pkgs"
IMAGE_FSTYPES=" cpio.gz"
