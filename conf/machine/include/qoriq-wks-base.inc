# short-description: Create eMMC/SD card image with a boot partition
#
# Offsets:
#   * RCW+PBI+BL2 at block 8
#   * PFE firmware at block 0x100
#   * FIP (BL31+BL32+BL33) at 0x800
#   * Env variables at 0x2800
#   * Secureboot headers at 0x3000
#   * DDR PHY FIP at 0x4000
#   * DPAA1 FMAN ucode at 0x4800
#   * DPAA2-MC at 0x5000
#   * DPAA2 DPL at 0x6800
#   * DPAA2 DPC at 0x7000
#   * Device tree (UEFI) at 0x7800
#   * Kernel at 0x8000
#   * Ramdisk at 0x10000
#
# The disk layout (in blocks) used is:
#  blocks                           KiB
#     0x0 +-----------------------+    0x0
#         | MBR / GPT ...         |
#     0x8 +-----------------------+    0x4a
#         | RCW + PBI + BL2       |
#   0x100 +-----------------------+   0x80
#         | PFE firmware          |
#   0x800 +-----------------------+  0x400
#         | FIP (BL31+BL32+BL33)  |
#  0x2800 +-----------------------+ 0x1400
#         | U-Boot Env variables  |
#  0x3000 +-----------------------+ 0x1800
#         | Secureboot headers    |
#  0x4000 +-----------------------+ 0x2000
#         | DDR PHY FIP           |
#  0x4800 +-----------------------+ 0x2400
#         | DPAA1 FMAN ucode      |
#  0x5000 +-----------------------+ 0x2800
#         | DPAA2-MC              |
#  0x6800 +-----------------------+ 0x3400
#         | DPAA2 DPL             |
#  0x7000 +-----------------------+ 0x3800
#         | DPAA2 DPC             |
#  0x7800 +-----------------------+ 0x3c00
#         | Device tree (UEFI)    |
#  0x8000 +-----------------------+ 0x4000
#         | Kernel                |
# 0x10000 +-----------------------+ 0x8000
#         | Ramdisk               |
#         +-----------------------+
#

# offset(1K) number in beginning 64MB space of SD/eMMC card on all QorIQ platforms

SD_PBL_OFFSET  ?= "4"
SD_FIP_OFFSET  ?= "1024"
SD_UBOOT_ENV_OFFSET  ?= "5120"
SD_SECUREBOOT_HEADERS_OFFSET  ?= "6144"
SD_DDR_PHY_FW_OFFSET  ?= "8192"
SD_FUSE_HEADERS_OFFSET  ?= "8704"
SD_FMAN_UCODE_OFFSET  ?= "9216"
SD_QE_FIRMWARE_OFFSET  ?= "9472"
SD_DP_FIRMWARE_OFFSET  ?= "9472"
SD_PHY_FIRMWARE_OFFSET  ?= "9728"
SD_UBOOT_SCR_OFFSET  ?= "9984"
SD_DPAA2_MC_FW_OFFSET  ?= "10240"
SD_DPAA2_MC_DPL_OFFSET  ?= "13312"
SD_DPAA2_MC_DPC_OFFSET  ?= "14336"
SD_DEVICE_TREE_OFFSET  ?= "15360"
SD_KERNEL_OFFSET  ?= "16384"
SD_KERNELITB_OFFSET  ?= "16384"
SD_RAMDISKRFS_OFFSET  ?= "17408"


PBL_FILE ?= "atf/bl2_sd.pbl"
FIP_FILE ?= "atf/fip_uboot.bin"
DDR_PYH_FILE ?= "ddr-phy/fip_ddr_all.bin"
FMAN_UCODE_FILE ?= ""
QE_DP_FILE ?= ""
ETH_PHY_FILE ?= "ls2-phy/cs4315-cs4340-PHY-ucode.txt"
MCFW_FILE ?= "mc_app/mc.itb"
DPL_FILE ?= ""
DPC_FILE ?= ""
ITB_FILE ?= ""

SOC_DEFAULT_IMAGE_FSTYPES += " wic.bmap wic.gz"

# Do not update fstab file when using wic images
WIC_CREATE_EXTRA_ARGS ?= "--no-fstab-update"

IMAGE_BOOT_FILES ?= " \
    ${KERNEL_IMAGETYPE} \
    *.dtb \
"

SOC_DEFAULT_WKS_FILE_fsl-lsch2 ?= "qoriq-lsch2.wks.in"
SOC_DEFAULT_WKS_FILE_fsl-lsch3 ?= "qoriq-lsch3.wks.in"

WKS_FILE ?= "${SOC_DEFAULT_WKS_FILE}"
