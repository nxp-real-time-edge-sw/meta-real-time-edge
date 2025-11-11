# Copyright 2020-2021 NXP

SUMMARY = "OPC UA implementation"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

SRCBRANCH ?= "master"
SRC_URI = "gitsm://github.com/open62541/open62541.git;protocol=https;branch=${SRCBRANCH} \
           file://0001-feat-examples-Add-pubsub-TSN-sample-applications.patch \
           file://0002-feat-examples-Add-OPC-UA-PUBSUB-summation-example-ap.patch \
"

PARALLEL_MAKE:arm = "-j1"
BB_NUMBER_THREADS:arm = "1"

EXTRA_OECMAKE:append:arm = " \
    -DUA_ENABLE_AMALGAMATION=OFF \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DUA_NAMESPACE_ZERO=REDUCED \
    -DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS=OFF \
"

# Modify these as desired
PV = "v1.2.2"
SRCREV = "ecf5a703785877a8719a0cda863a98455f7d5d12"

S = "${WORKDIR}/git"

DEPENDS = "openssl"

ERROR_QA:remove = "buildpaths"
WARN_QA:append = " buildpaths"

do_install:append () {
	if [ -d ${B}/bin/examples ] && [ -z $(find ${B}/bin/examples -maxdepth 0 -empty) ]
	then
		install -d ${D}${ROOT_HOME}/open62541_example/
		install -m 0755 ${B}/bin/examples/* ${D}${ROOT_HOME}/open62541_example/
	fi
}

FILES:${PN} += "${bindir_native}/* ${datadir_native}/open62541/* ${libdir_native}/* ${ROOT_HOME}/*"

inherit cmake python3native

