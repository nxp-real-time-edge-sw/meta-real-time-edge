SUMMARY = "GenAVB/TSN Stack Kernel Modules"

require genavb-tsn.inc

inherit module

GENAVB_TSN_MODULES_BUILD_DIR="build/${GENAVB_TSN_TARGET}/${GENAVB_TSN_CONFIG}"
GENAVB_TSN_MODULES_OBJ_DIR="${B}/${GENAVB_TSN_MODULES_BUILD_DIR}"

MODULES_MODULE_SYMVERS_LOCATION="${GENAVB_TSN_MODULES_BUILD_DIR}"

EXTRA_OEMAKE += " \
    -C ${GENAVB_TSN_MODULES_OBJ_DIR} \
    CROSS_COMPILE=${TARGET_PREFIX} \
    GENAVB_INCLUDE=${S}/include \
    KERNELDIR=${STAGING_KERNEL_DIR} \
"

do_configure:append() {
    # Generate modules sources in custom directory ${GENAVB_TSN_MODULES_OBJ_DIR}
    # To be used later as make execute directory for the following tasks
    ${MAKE} \
        -C ${S}/linux/modules/ \
        MODULES_OBJ_DIR=${GENAVB_TSN_MODULES_OBJ_DIR} \
        config=${GENAVB_TSN_CONFIG} target=${GENAVB_TSN_TARGET} \
        modules_sources
}
