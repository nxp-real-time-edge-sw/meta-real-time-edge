# Copyright 2020-2021 NXP

SUMMARY = "OPC UA implementation"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

SRCBRANCH ?= "1.3"
SRC_URI = "gitsm://github.com/open62541/open62541.git;protocol=https;branch=${SRCBRANCH} \
           file://0001-feat-examples-Add-OPC-UA-PubSub-publisher-subscriber.patch \
           file://0002-feat-examples-Add-OPC-UA-PUBSUB-summation-example-ap.patch \
"

# Build the library statically. The NXP real-time PubSub example apps
# (opcua_pubsub_publisher, opcua_pubsub_subscriber, opcua_summation_controller,
# opcua_summation_device and the pubsub_TSN_* samples) link the internal
# open62541 object files ($<TARGET_OBJECTS:open62541-object>) and use internal
# headers. Upstream only compiles these examples when BUILD_SHARED_LIBS is OFF;
# with a shared library they are skipped. Force the static build here so the
# examples are actually built and installed.
LIBOPEN62541_BUILD_SHARED_LIBS = "OFF"

# PubSub over Ethernet (UADP) is required by the NXP example applications.
EXTRA_OECMAKE:append = " \
    -DUA_ENABLE_COVERAGE=OFF \
    -DUA_ENABLE_PUBSUB=ON \
    -DUA_ENABLE_PUBSUB_ETH_UADP=ON \
"


EXTRA_OECMAKE:append:arm = " \
    -DUA_ENABLE_AMALGAMATION=OFF \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DUA_NAMESPACE_ZERO=REDUCED \
    -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS=OFF \
"

# Modify these as desired
PV = "v1.3.17"
SRCREV = "41f4deef34a9d0f94fcb830e2c831a9eb6236ade"

DEPENDS = "openssl"

ERROR_QA:remove = "buildpaths"
WARN_QA:append = " buildpaths"

# disable strip
INHIBIT_PACKAGE_STRIP = "1"
INHIBIT_PACKAGE_DEBUG_SPLIT = "1"
INHIBIT_SYSROOT_STRIP = "1"

do_install:append () {
	if [ -d ${B}/bin/examples ] && [ -z $(find ${B}/bin/examples -maxdepth 0 -empty) ]
	then
		install -d ${D}${ROOT_HOME}/open62541_example/
		install -m 0755 ${B}/bin/examples/* ${D}${ROOT_HOME}/open62541_example/
	fi
}

FILES:${PN} += "${bindir_native}/* ${datadir_native}/open62541/* ${libdir_native}/* ${ROOT_HOME}/*"

inherit cmake python3native
