# Copyright 2020-2021 NXP

SUMMARY = "OPC UA implementation"
LICENSE = "MPL-2.0"
LIC_FILES_CHKSUM = "file://LICENSE;md5=815ca599c9df247a0c7f619bab123dad"

SRC_URI = "gitsm://github.com/open62541/open62541.git;protocol=https"

# Modify these as desired
PV = "v1.2.2"
SRCREV = "ecf5a703785877a8719a0cda863a98455f7d5d12"

S = "${WORKDIR}/git"

DEPENDS = "openssl"

# *************** BUILD_TYPE ***************
#   RelWithDebInfo :  -O2 optimization with debug symbols
#   Release        :  -O2 optimization without debug symbols
#   Debug          :  -O0 optimization with debug symbols
#   MinSizeRel     :  -Os optimization without debug symbols
UA_BUILD_TYPE ??= "Release"

# *************** LOGLEVEL ***************
#  600: Fatal
#  500: Error
#  400: Warning
#  300: Info
#  200: Debug
#  100: Trace
UA_LOGLEVE ??= "600"

# *************** UA_MULTITHREADING ***************
#  Level of multi-threading support.
#  0-99     : Multithreading support disabled.
#  100-199  : API functions marked with the UA_THREADSAFE-macro are protected
#             internally with mutexes. Multiple threads are allowed to call these
#             functions of the SDK at the same time without causing race conditions.
#             Furthermore, this level support the handling of asynchronous method
#             calls from external worker threads.
#  >=200    : Work is distributed to a number of internal worker threads.
#             Currently only used for mDNS discovery. (EXPERIMENTAL FEATURE! Expect bugs.)
UA_MULTITHREADING ??= "200"

# *************** BUILD_EXAMPLES ***************
# Compile example servers and clients from examples/*.c.
# ON  : Enable
# OFF : Disable
BUILD_EXAMPLES ??= "ON"

# *************** BUILD_UNIT_TESTS ***************
# Compile unit tests.
# ON  : Enable
# OFF : Disable
BUILD_UNIT_TESTS ??= "OFF"


# *************** BUILD_SELFSIGNED_CERTIFICATE ***************
# Generate a self-signed certificate for the server (openSSL required)
# ON  : Enable
# OFF : Disable
BUILD_SELFSIGNED_CERTIFICATE ??= "ON"


# *************** ENABLE_SUBSCRIPTIONS ***************
# Enable subscriptions.
# ON  : Enable
# OFF : Disable
ENABLE_SUBSCRIPTIONS ??= "ON"

# *************** ENABLE_SUBSCRIPTIONS_EVENTS ***************
# Enable the use of events for subscriptions.
# ON  : Enable
# OFF : Disable
ENABLE_SUBSCRIPTIONS_EVENTS  ??= "ON"

# ************** ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS ********
# Enable the use of A&C for subscriptions.
# ON  : Enable
# OFF : Disable
ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS  ??= "ON"

# ************** ENABLE_METHODCALLS ******************
# Enable the Method service set
# ON  : Enable
# OFF : Disable
ENABLE_METHODCALLS ??= "ON"

# ************* ENABLE_PARSING ***************
# Enable parsing human readable formats of builtin data types (Guid, NodeId, etc.).
# ON  : Enable
# OFF : Disable
ENABLE_PARSING ??= "ON"

# ************ ENABLE_NODEMANAGEMENT ****************
# Enable dynamic addition and removal of nodes at runtime.
# ON  : Enable
# OFF : Disable
ENABLE_NODEMANAGEMENT ??= "ON"

# *********** ENABLE_AMALGAMATION *********
# Compile a single-file release into the files open62541.c and open62541.h.
# ON  : Enable
# OFF : Disable
ENABLE_AMALGAMATION ??= "OFF"

# *********** ENABLE_IMMUTABLE_NODES ********
# Nodes in the information model are not edited but copied and replaced.
# ON  : Enable
# OFF : Disable
ENABLE_IMMUTABLE_NODES ??= "OFF"

# ********** ENABLE_COVERAGE *********
# Measure the coverage of unit tests.
# ON  : Enable
# OFF : Disable
ENABLE_COVERAGE ??= "ON"

# ********** ENABLE_DISCOVERY *********
# Enable Discovery Service (LDS).
# ON  : Enable
# OFF : Disable
ENABLE_DISCOVERY ??= "ON"

# ********** ENABLE_DISCOVERY_MULTICAST *********
# Enable Discovery Service with multicast support (LDS-ME).
# ON  : Enable
# OFF : Disable
ENABLE_DISCOVERY_MULTICAST ??= "ON"

# ********** ENABLE_DISCOVERY_SEMAPHORE *********
# Enable Discovery Semaphore support.
# ON  : Enable
# OFF : Disable
ENABLE_DISCOVERY_SEMAPHORE ??= "ON"

# ********** NAMESPACE_ZERO *********
# Namespace zero contains the standard-defined nodes. The full namespace
# zero may not be required for all applications.
# MINIMAL     :  A barebones namespace zero that is compatible with most clients.
#                But this namespace 0 is so small that it does not pass the CTT
#                (Conformance Testing Tools of the OPC Foundation).
# REDUCED    :  Small namespace zero that passes the CTT.
# FULL       :  Full namespace zero generated from the official XML definitions.
NAMESPACE_ZERO ??= "FULL"

# ********** ENABLE_TYPEDESCRIPTION *********
# Add the type and member names to the UA_DataType structure.
# ON  : Enable
# OFF : Disable
ENABLE_TYPEDESCRIPTION ??= "ON"

# ********** ENABLE_STATUSCODE_DESCRIPTIONS *********
# Compile the human-readable name of the StatusCodes into the binary.
# ON  : Enable
# OFF : Disable
ENABLE_STATUSCODE_DESCRIPTIONS ??= "ON"

# ********** ENABLE_FULL_NS0 *********
# Use the full NS0 instead of a minimal Namespace 0 nodeset UA_FILE_NS0 is used to
# specify the file for NS0 generation from namespace0 folder.
ENABLE_FULL_NS0 ??= ""

# ********** BUILD_SHARED_LIBS *******
# ON  : Enable
# OFF : Disable
BUILD_SHARED_LIBS ??= "ON"

# ********** BUILD_DEBUG *******
# Enable assertions and additional definitions not intended for production builds
# ON  : Enable
# OFF : Disable
BUILD_DEBUG ??= "OFF"

# ********** DEBUG_DUMP_PKGS *******
# Dump every package received by the server as hexdump format.
# ON  : Enable
# OFF : Disable
DEBUG_DUMP_PKG ??= "OFF"

EXTRA_OECMAKE += " \
	-DCMAKE_BUILD_TYPE=${UA_BUILD_TYPE} \
	-DUA_LOGLEVEL=${UA_LOGLEVE} \
	-DUA_MULTITHREADING=${UA_MULTITHREADING} \
	-DUA_BUILD_UNIT_TESTS=${BUILD_UNIT_TESTS} \
	-DUA_BUILD_SELFSIGNED_CERTIFICATE=${BUILD_SELFSIGNED_CERTIFICATE} \
	-DUA_ENABLE_SUBSCRIPTIONS=${ENABLE_SUBSCRIPTIONS} \
	-DUA_ENABLE_SUBSCRIPTIONS_EVENTS=${ENABLE_SUBSCRIPTIONS_EVENTS} \
	-DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS=${ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS} \
	-DUA_ENABLE_METHODCALLS=${ENABLE_METHODCALLS} \
	-DUA_ENABLE_PARSING=${ENABLE_PARSING} \
	-DUA_ENABLE_NODEMANAGEMENT=${ENABLE_NODEMANAGEMENT} \
	-DUA_ENABLE_AMALGAMATION=${ENABLE_AMALGAMATION} \
	-DUA_ENABLE_IMMUTABLE_NODES=${ENABLE_IMMUTABLE_NODES} \
	-DUA_ENABLE_COVERAGE=${ENABLE_COVERAGE} \
	-DUA_ENABLE_DISCOVERY=${ENABLE_DISCOVERY} \
	-DUA_ENABLE_DISCOVERY_MULTICAST=${ENABLE_DISCOVERY_MULTICAST} \
	-DUA_ENABLE_DISCOVERY_SEMAPHORE=${ENABLE_DISCOVERY_SEMAPHORE} \
	-DUA_NAMESPACE_ZERO=${NAMESPACE_ZERO} \
	-DUA_ENABLE_TYPEDESCRIPTION=${ENABLE_TYPEDESCRIPTION} \
	-DUA_ENABLE_STATUSCODE_DESCRIPTIONS=${ENABLE_STATUSCODE_DESCRIPTIONS} \
	-DUA_FILE_NS0=${ENABLE_FULL_NS0} \
	-DBUILD_SHARED_LIBS=${BUILD_SHARED_LIBS} \
	-DUA_BUILD_EXAMPLES=${BUILD_EXAMPLES} \
	-DUA_DEBUG=${BUILD_DEBUG} \
	-DUA_DEBUG_DUMP_PKGS=${DEBUG_DUMP_PKG} \
"
do_install_append () {
	install -d ${D}${ROOT_HOME}/open62541_example/
	install -m 644 ${B}/bin/examples/* ${D}${ROOT_HOME}/open62541_example/
}

FILES_${PN} += "${bindir_native}/* ${datadir_native}/open62541/* ${libdir_native}/* ${ROOT_HOME}/*"

inherit cmake python3native

