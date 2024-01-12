# Copyright 2022-2024 NXP

require arm-binary-toolchain.inc

COMPATIBLE_HOST = "(x86_64|aarch64).*-linux"

SUMMARY = "Baremetal GCC for Aarch64 processors"
LICENSE = "GPL-3.0-with-GCC-exception & GPL-3.0-only"

LIC_FILES_CHKSUM:aarch64 = "file://share/doc/gcc/Copying.html;md5=be4f8b5ff7319cd54f6c52db5d6f36b0"
LIC_FILES_CHKSUM:x86-64 = "file://share/doc/gcc/Copying.html;md5=1f07179249795891179bb3798bac7887"

PROVIDES = "virtual/aarch64-none-elf-gcc"

SRC_URI = "https://developer.arm.com/-/media/Files/downloads/gnu/${PV}/binrel/gcc-arm-${PV}-${HOST_ARCH}-${BINNAME}.tar.xz;name=gnu-${HOST_ARCH}"
SRC_URI[gnu-aarch64.sha256sum] = "3b15725545a0211a17b63e72d4f10241f7ffbe7ce94cb9612590ceacde16992c"
SRC_URI[gnu-x86_64.sha256sum] = "b0a015a9e8cbb44ed2fe5ad755a7a7ae254d54f93df3bf47378485b0ba8b828b"

S = "${WORKDIR}/gcc-arm-${PV}-${HOST_ARCH}-${BINNAME}"

UPSTREAM_CHECK_URI = "https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-a/downloads"
UPSTREAM_CHECK_REGEX = "gcc-arm-(?P<pver>.+)-${HOST_ARCH}-${BINNAME}\.tar\.\w+"
