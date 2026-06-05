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
#   Enable fastboot by adding to local.conf:
#     DISTRO_FEATURES:append = " fastboot"
#
#   Default: Two RTOS mode (no U-Boot):
#     Core 0: hello_world RTOS at 0xD0000000
#     Core 1: hello_world RTOS at 0xD1000000
#
#   Customize RTOS images in local.conf (full path relative to DEPLOY_DIR_IMAGE):
#     CA_CORE0_IMG = "examples/heterogeneous-multicore/rt-latency-freertos/rt_latency_ca55_RTOS0_UART2.bin"
#     CA_CORE1_IMG = "examples/heterogeneous-multicore/hello-world-zephyr/hello_world_ca55_RTOS1_UART1.bin"
#
#   U-Boot + single RTOS mode:
#     CA_CORE0_IMG = ""
#     CA_CORE1_IMG = "examples/heterogeneous-multicore/hello-world-freertos/hello_world_ca55_RTOS0_UART2.bin"
#     CA_CORE1_IMG_LOAD_ADDR = "0xD0000000"

# Fastboot feature flag (controlled by DISTRO_FEATURES)
FASTBOOT_ENABLED = "${@bb.utils.contains('DISTRO_FEATURES', 'fastboot', '1', '0', d)}"

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
# - "yes": Embed fastboot entries in U-Boot (when Core 0 runs U-Boot)
# - "no": Use standalone fastboot entries file (when Core 0 runs custom image)
# - Auto-detected based on CA_CORE0_IMG
FASTBOOT_ENTRY_EMBEDDED_UBOOT ?= "${@'no' if d.getVar('CA_CORE0_IMG') else 'yes'}"

# Fastboot config file (maps to fastboot=<file> in make command)
# If set, this .mak file is passed to imx-mkimage instead of individual variables
FASTBOOT_CONFIG ?= ""

#
# Platform-specific defaults
#

# i.MX93: Default Two RTOS mode (Core 0 + Core 1, no U-Boot)
CA_CORE0_IMG:mx93-nxp-bsp ?= "examples/heterogeneous-multicore/hello-world-freertos/hello_world_ca55_RTOS1_UART1.bin"
CA_CORE0_IMG_LOAD_ADDR:mx93-nxp-bsp ?= "0xD0000000"
CA_CORE1_IMG:mx93-nxp-bsp ?= "examples/heterogeneous-multicore/hello-world-freertos/hello_world_ca55_RTOS0_UART2.bin"
CA_CORE1_IMG_LOAD_ADDR:mx93-nxp-bsp ?= "0xD1000000"
