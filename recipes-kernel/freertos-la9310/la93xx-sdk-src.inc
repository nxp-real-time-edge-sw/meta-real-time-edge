# Copyright 2023-2024 NXP

require ../include/la93xx-repo.inc

LICENSE = "Apache-2.0"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/Apache-2.0;md5=89aea4e17d99a7cacdbeed46a0096b10"

# This file might be included from other places (like other layers) and not
# having an explicit path to the patches directory, will make bitbake fail to
# find the patch(es) in SRC_URI.
FILESEXTRAPATHS:prepend := "${THISDIR}/files:"

NXPMICRO_BASE = "github.com/nxp-mcuxpresso"
NXP_BASE = "github.com/NXP"
NXP_REAL_TIME_EDGE_BASE="github.com/nxp-real-time-edge-sw"

SRCREV_FORMAT = "la93xx-sdk"

SRC_URI = "\
    ${SRC_LA9310_FRTOS_URI};name=freertos_repo \
    ${SRC_LA9310_FW_URI};name=firmware_repo;destsuffix=git/firmware \
    "

S = "${WORKDIR}/git"

# Default to a stable version
PREFERRED_VERSION_MCUX-SDK ??= "2.12.0"
#require la93xx-sdk-src-${PREFERRED_VERSION_MCUX-SDK}.inc

PACKAGES = "${PN}"
