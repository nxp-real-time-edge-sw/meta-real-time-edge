SUMMARY = "GenAVB/TSN Stack Kernel Module"

require genavb-tsn.inc

inherit module

MODULES_MODULE_SYMVERS_LOCATION="linux/modules/build/${GENAVB_TSN_TARGET}/${GENAVB_TSN_CONFIG}"

EXTRA_OEMAKE += " \
    -C ${S}/linux/modules/ \
    CROSS_COMPILE=${TARGET_PREFIX} \
    KERNELDIR=${STAGING_KERNEL_DIR} \
    target=${GENAVB_TSN_TARGET} \
    config=${GENAVB_TSN_CONFIG} \
    PREFIX=${D} \
"
