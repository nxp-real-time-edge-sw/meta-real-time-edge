# Copyright 2026 NXP
# Multicore Fastboot Configuration for i.MX platforms
#
# This file defines variables for configuring multicore fastboot,
# which allows booting multiple RTOS images on different Cortex-A cores.
#
# Variable naming matches imx-mkimage parameters for consistency:
#   CA_CORE<N>_IMG           -> CA_CORE<N>_IMG
#   CA_CORE<N>_IMG_LOAD_ADDR -> CA_CORE<N>_IMG_LOAD_ADDR
#
# Boot Sequence:
#   Secondary cores (Core 1, 2, 3, ...) boot first in ascending order
#   Primary core (Core 0) boots last (U-Boot or custom image)
#
# Usage:
#   Fastboot is implemented as an imx-boot variant (IMXBOOT_VARIANT), aligned
#   with the BSP variant mechanism used by netc/jailhouse/etc. Enable it by
#   selecting the fastboot variant in local.conf:
#     IMXBOOT_VARIANT = "fastboot"
#
#   This produces a dedicated boot artifact named with the variant suffix, e.g.
#     imx-boot-variant-fastboot-imx93evk-sd.bin-flash_singleboot
#   Each bitbake invocation builds a single variant (same as netc/jailhouse).
#
#   Default: Two RTOS mode (no U-Boot):
#     Core 0: hello_world RTOS1 binary at 0xD1000000
#     Core 1: hello_world RTOS0 binary at 0xD0000000
#
#   Note the crossed naming: Core 0 uses an RTOS1 binary and Core 1 uses an RTOS0
#   binary. This is intentional and matches the boot-order semantics (Core 1 boots
#   first, Core 0 boots last); always configure by CA_CORE<N>, not by the RTOS<n>
#   substring in the file name.
#
#   The HMC example recipes deploy their RTOS binaries to
#   ${DEPLOY_DIR_IMAGE}/examples/${RTOS_MACHINE}/heterogeneous-multicore/<recipe>-<rtos>/
#   (see HMC_EXAMPLE_INSTALL_DIR in meta-rtos-industrial), so the CA_CORE*_IMG
#   paths below are machine-relative via examples/${RTOS_MACHINE}.
#
#   Customize RTOS images in local.conf (full path):
#     CA_CORE0_IMG = "/path/to/your/ca55_rtos1_img.bin"
#     CA_CORE1_IMG = "/path/to/your/ca55_rtos0_img.bin"
#
#   U-Boot + single RTOS mode:
#     CA_CORE0_IMG = ""
#     CA_CORE1_IMG = "/path/to/your/ca55_rtos0_img.bin"
#     CA_CORE1_IMG_LOAD_ADDR = "0xD0000000"

# Register the fastboot variant in the machine's allowed variant list so that
# BOOT_VARIANT (defined in imx-base-extend.inc) resolves to "-variant-fastboot"
# when IMXBOOT_VARIANT = "fastboot".
IMXBOOT_VARIANTS:append:mx93-nxp-bsp = " fastboot"

# Fastboot feature flag (driven by the IMXBOOT_VARIANT selection)
FASTBOOT_ENABLED = "${@bb.utils.contains('IMXBOOT_VARIANT', 'fastboot', '1', '0', d)}"

# Core 0 configuration (empty = U-Boot on Core 0)
# Full path relative to DEPLOY_DIR_IMAGE
CA_CORE0_IMG ?= ""
CA_CORE0_IMG_LOAD_ADDR ?= ""

# Core 1 configuration
CA_CORE1_IMG ?= ""
CA_CORE1_IMG_LOAD_ADDR ?= ""

# Additional cores (for future platforms with more than 2 A-cores)
CA_CORE2_IMG ?= ""
CA_CORE2_IMG_LOAD_ADDR ?= ""
CA_CORE3_IMG ?= ""
CA_CORE3_IMG_LOAD_ADDR ?= ""

# Fastboot entry embedding mode (matches imx-mkimage parameter)
FASTBOOT_ENTRY_EMBEDDED_UBOOT ?= ""

# Fastboot config file (maps to fastboot=<file> in make command)
# If set, this .mak file is passed to imx-mkimage instead of individual variables
FASTBOOT_CONFIG ?= ""

#
# Platform-specific defaults
#

RTOS_MACHINE ?= "${MACHINE}"
RTOS_MACHINE:imx93evk = "imx93-11x11-lpddr4x-evk"

# i.MX93: Default Two RTOS mode (Core 0 + Core 1, no U-Boot)
# Paths are relative to DEPLOY_DIR_IMAGE and match the HMC example deploy layout
# examples/${RTOS_MACHINE}/heterogeneous-multicore/<recipe>-<rtos>/.
CA_CORE0_IMG:mx93-nxp-bsp ?= "${DEPLOY_DIR_IMAGE}/examples/${RTOS_MACHINE}/heterogeneous-multicore/hello-world-freertos/hello_world_ca55_RTOS1_UART1.bin"
CA_CORE0_IMG_LOAD_ADDR:mx93-nxp-bsp ?= "0xD1000000"
CA_CORE1_IMG:mx93-nxp-bsp ?= "${DEPLOY_DIR_IMAGE}/examples/${RTOS_MACHINE}/heterogeneous-multicore/hello-world-freertos/hello_world_ca55_RTOS0_UART2.bin"
CA_CORE1_IMG_LOAD_ADDR:mx93-nxp-bsp ?= "0xD0000000"
