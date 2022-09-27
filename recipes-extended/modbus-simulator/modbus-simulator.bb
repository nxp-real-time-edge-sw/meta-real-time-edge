SUMMARY = "Modbus simulator"
DESCRIPTION = "Implementation of modbus-simulator based on libmodbus"
LICENSE = "GPL-2.0-only & MIT"
LIC_FILES_CHKSUM = "file://LICENSE.md;md5=a1074b08ac5859eeab6a4d0bd23bd281"

SRC_URI = "file://LICENSE.md \
           file://CMakeLists.txt \
           file://modbus_client_simulator.c \
           file://modbus_device_simulator.c \
           file://common.c \
           file://common.h \
           file://readme.md"

S = "${WORKDIR}"

DEPENDS = "libmodbus"

inherit cmake pkgconfig
