# Copyright 2026 NXP
# Multicore Fastboot support for imx-boot
#
# This bbappend adds support for building boot images with multicore fastboot,
# allowing multiple RTOS images to be loaded on different Cortex-A cores.
#
# Enable by selecting the fastboot imx-boot variant in local.conf:
#   IMXBOOT_VARIANT = "fastboot"
#
# This is aligned with the BSP variant mechanism (BOOT_VARIANT in
# imx-base-extend.inc) used by netc/jailhouse/etc. Each bitbake invocation
# builds a single variant, and the produced artifacts carry the
# -variant-fastboot suffix. The make targets are driven by UBOOT_CONFIG (not by
# the variant), so an i.MX93 SD build iterates three IMXBOOT_TARGETS and deploys
# three suffixed artifacts, e.g.
#   imx-boot-variant-fastboot-imx93evk-sd.bin-flash_singleboot
#   imx-boot-variant-fastboot-imx93evk-sd.bin-flash_singleboot_gdet
#   imx-boot-variant-fastboot-imx93evk-sd.bin-flash_singleboot_gdet_auto

FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

# imx-boot and imx-mkimage are separate recipes that each unpack their own copy
# of the imx-mkimage source tree. The fastboot patch must be applied to the
# imx-boot source as well, since imx-boot is the recipe that runs the
# imx-mkimage build target to generate the boot image.
SRC_URI:append = " \
    file://0001-iMX93-add-fastboot-support.patch \
"

# When fastboot is enabled, the RTOS firmware images are concatenated into the
# boot binary by imx-mkimage. Those RTOS images are produced by a separate recipe
# (the HMC examples) and carry their own debug build paths (e.g. references to
# work-shared/hmc-source/.../freertos-kernel/*.c). imx-boot cannot strip paths
# from inside the opaque concatenated boot image, so the buildpaths QA check would
# fail on the embedded firmware. Skip that check only when fastboot is enabled.
INSANE_SKIP:${PN} += "${@'buildpaths' if d.getVar('FASTBOOT_ENABLED') == '1' else ''}"

# Fastboot dependencies - require all HMC RTOS examples when fastboot is enabled
FASTBOOT_HMC_DEPENDS = ""

FASTBOOT_HMC_DEPENDS:mx93-nxp-bsp = "${@'hello-world:do_deploy \
     rt-latency:do_deploy \
     rpmsg-str-echo:do_deploy \
     rpmsg-pingpong:do_deploy \
     virtio-perf:do_deploy \
     virtio-net-backend:do_deploy' if d.getVar('FASTBOOT_ENABLED') == '1' else ''}"

do_compile[depends] += "${FASTBOOT_HMC_DEPENDS}"

# Validate fastboot configuration
python do_validate_fastboot() {
    """Validate fastboot configuration before compile"""
    if d.getVar('FASTBOOT_ENABLED') != '1':
        return

    errors = []
    warnings = []

    # Check each core's configuration
    for core_id in range(4):
        img_var = f'CA_CORE{core_id}_IMG'
        addr_var = f'CA_CORE{core_id}_IMG_LOAD_ADDR'
        
        img = d.getVar(img_var)
        addr = d.getVar(addr_var)
        
        if img and not addr:
            errors.append(f"{img_var} is set to '{img}' but {addr_var} is not set")

    # Validate embedding configuration
    embedded = d.getVar('FASTBOOT_ENTRY_EMBEDDED_UBOOT')
    core0_img = d.getVar('CA_CORE0_IMG')
    
    if embedded == 'no' and not core0_img:
        errors.append("FASTBOOT_ENTRY_EMBEDDED_UBOOT=no requires CA_CORE0_IMG to be set")
    
    if embedded == 'yes' and core0_img:
        warnings.append(f"Conflicting config: CA_CORE0_IMG='{core0_img}' but FASTBOOT_ENTRY_EMBEDDED_UBOOT=yes")

    for warn in warnings:
        bb.warn(f"Fastboot: {warn}")
    
    if errors:
        for err in errors:
            bb.error(f"Fastboot: {err}")
        bb.fatal("Fastboot configuration validation failed")
}
addtask validate_fastboot before do_compile after do_configure

# Copy RTOS images to boot staging directory
# Image path: ${DEPLOY_DIR_IMAGE}/${CA_CORE*_IMG}
do_compile:prepend:mx93-nxp-bsp() {
    if [ "${FASTBOOT_ENABLED}" = "1" ]; then
        bbnote "Fastboot: Preparing RTOS images for multicore boot"
        
        # Core 0 RTOS image -> ca55_rtos0_img.bin (boots last)
        if [ -n "${CA_CORE0_IMG}" ]; then
            RTOS_IMG="${DEPLOY_DIR_IMAGE}/${CA_CORE0_IMG}"
            if [ -f "${RTOS_IMG}" ]; then
                cp "${RTOS_IMG}" ${BOOT_STAGING}/ca55_rtos0_img.bin
                bbnote "Fastboot: Core 0 image: ${RTOS_IMG} -> ca55_rtos0_img.bin"
            else
                bbfatal "Fastboot: Core 0 RTOS image not found: ${RTOS_IMG}"
            fi
        fi

        # Core 1 RTOS image -> ca55_rtos1_img.bin (boots first)
        if [ -n "${CA_CORE1_IMG}" ]; then
            RTOS_IMG="${DEPLOY_DIR_IMAGE}/${CA_CORE1_IMG}"
            if [ -f "${RTOS_IMG}" ]; then
                cp "${RTOS_IMG}" ${BOOT_STAGING}/ca55_rtos1_img.bin
                bbnote "Fastboot: Core 1 image: ${RTOS_IMG} -> ca55_rtos1_img.bin"
            else
                bbfatal "Fastboot: Core 1 RTOS image not found: ${RTOS_IMG}"
            fi
        fi
    fi
}

# Generate fastboot arguments for imx-mkimage
def get_fastboot_mkimage_args(d):
    """Generate imx-mkimage command line arguments for fastboot"""
    if d.getVar('FASTBOOT_ENABLED') != '1':
        return ''

    args = []
    
    # If config file specified, use it
    fastboot_config = d.getVar('FASTBOOT_CONFIG')
    if fastboot_config:
        args.append(f'fastboot={fastboot_config}')
        return ' '.join(args)

    # Build arguments from individual variables
    # Core 0 (custom image instead of U-Boot)
    core0_img = d.getVar('CA_CORE0_IMG')
    core0_addr = d.getVar('CA_CORE0_IMG_LOAD_ADDR')
    if core0_img and core0_addr:
        args.append('CA_CORE0_IMG=ca55_rtos0_img.bin')
        args.append(f'CA_CORE0_IMG_LOAD_ADDR={core0_addr}')
        args.append('FASTBOOT_ENTRY_EMBEDDED_UBOOT=no')

    # Core 1
    core1_img = d.getVar('CA_CORE1_IMG')
    core1_addr = d.getVar('CA_CORE1_IMG_LOAD_ADDR')
    if core1_img and core1_addr:
        args.append('CA_CORE1_IMG=ca55_rtos1_img.bin')
        args.append(f'CA_CORE1_IMG_LOAD_ADDR={core1_addr}')

    return ' '.join(args)

# Append fastboot arguments to MKIMAGE_EXTRA_ARGS.
# Since each bitbake invocation builds a single IMXBOOT_VARIANT, these args only
# take effect in the fastboot-variant build and never leak into a normal build.
MKIMAGE_EXTRA_ARGS:append:mx93-nxp-bsp = " ${@get_fastboot_mkimage_args(d)}"

# Deploy fastboot-related files
do_deploy:append:mx93-nxp-bsp() {
    if [ "${FASTBOOT_ENABLED}" = "1" ]; then
        bbnote "Fastboot: Deploying multicore boot artifacts"
        
        if [ -f "${BOOT_STAGING}/ca55_rtos0_img.bin" ]; then
            install -m 0644 ${BOOT_STAGING}/ca55_rtos0_img.bin ${DEPLOYDIR}/${BOOT_TOOLS}/
        fi
        if [ -f "${BOOT_STAGING}/ca55_rtos1_img.bin" ]; then
            install -m 0644 ${BOOT_STAGING}/ca55_rtos1_img.bin ${DEPLOYDIR}/${BOOT_TOOLS}/
        fi
    fi
}
