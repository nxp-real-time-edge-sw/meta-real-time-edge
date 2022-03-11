# Copyright 2020-2021 NXP

# *************** BUILD_TYPE ***************
#   RelWithDebInfo :  -O2 optimization with debug symbols
#   Release        :  -O2 optimization without debug symbols
#   Debug          :  -O0 optimization with debug symbols
#   MinSizeRel     :  -Os optimization without debug symbols
LIBOPEN62541_BUILD_TYPE ??= "Release"

# *************** LOGLEVEL ***************
#  600: Fatal
#  500: Error
#  400: Warning
#  300: Info
#  200: Debug
#  100: Trace
LIBOPEN62541_LOGLEVE ??= "300"

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
LIBOPEN62541_MULTITHREADING ??= "200"

# *************** BUILD_EXAMPLES ***************
# Compile example servers and clients from examples.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_BUILD_EXAMPLES ??= "ON"

# *************** BUILD_UNIT_TESTS ***************
# Compile unit tests.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_BUILD_UNIT_TESTS ??= "OFF"


# *************** ENABLE_SUBSCRIPTIONS ***************
# Enable subscriptions.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_SUBSCRIPTIONS ??= "ON"

# *************** ENABLE_SUBSCRIPTIONS_EVENTS ***************
# Enable the use of events for subscriptions.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_SUBSCRIPTIONS_EVENTS  ??= "ON"

# ************** ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS ********
# Enable the use of A&C for subscriptions.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS  ??= "ON"

# ************** ENABLE_METHODCALLS ******************
# Enable the Method service set
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_METHODCALLS ??= "ON"

# ************* ENABLE_PARSING ***************
# Enable parsing human readable formats of builtin data types (Guid, NodeId, etc.).
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_PARSING ??= "ON"

# ************ ENABLE_NODEMANAGEMENT ****************
# Enable dynamic addition and removal of nodes at runtime.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_NODEMANAGEMENT ??= "ON"

# *********** ENABLE_AMALGAMATION *********
# Compile a single-file release into the files open62541.c and open62541.h.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_AMALGAMATION ??= "OFF"

# *********** ENABLE_IMMUTABLE_NODES ********
# Nodes in the information model are not edited but copied and replaced.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_IMMUTABLE_NODES ??= "OFF"

# ********** ENABLE_COVERAGE *********
# Measure the coverage of unit tests.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_COVERAGE ??= "ON"

# ********** ENABLE_DISCOVERY *********
# Enable Discovery Service (LDS).
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_DISCOVERY ??= "ON"

# ********** ENABLE_DISCOVERY_MULTICAST *********
# Enable Discovery Service with multicast support (LDS-ME).
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_DISCOVERY_MULTICAST ??= "ON"

# ********** ENABLE_DISCOVERY_SEMAPHORE *********
# Enable Discovery Semaphore support.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_DISCOVERY_SEMAPHORE ??= "ON"

# ********** NAMESPACE_ZERO *********
# Namespace zero contains the standard-defined nodes. The full namespace
# zero may not be required for all applications.
# MINIMAL     :  A barebones namespace zero that is compatible with most clients.
#                But this namespace 0 is so small that it does not pass the CTT
#                (Conformance Testing Tools of the OPC Foundation).
# REDUCED    :  Small namespace zero that passes the CTT.
# FULL       :  Full namespace zero generated from the official XML definitions.
LIBOPEN62541_NAMESPACE_ZERO ??= "FULL"

# ********** ENABLE_TYPEDESCRIPTION *********
# Add the type and member names to the UA_DataType structure.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_TYPEDESCRIPTION ??= "ON"

# ********** ENABLE_STATUSCODE_DESCRIPTIONS *********
# Compile the human-readable name of the StatusCodes into the binary.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_STATUSCODE_DESCRIPTIONS ??= "ON"

# ********** ENABLE_FULL_NS0 *********
# Use the full NS0 instead of a minimal Namespace 0 nodeset UA_FILE_NS0 is used to
# specify the file for NS0 generation from namespace0 folder.
LIBOPEN62541_ENABLE_FULL_NS0 ??= ""

# ********** BUILD_SHARED_LIBS *******
# ON  : Enable
# OFF : Disable
LIBOPEN62541_BUILD_SHARED_LIBS ??= "ON"

# ********** BUILD_DEBUG *******
# Enable assertions and additional definitions not intended for production builds
# ON  : Enable
# OFF : Disable
LIBOPEN62541_BUILD_DEBUG ??= "OFF"

# ********** DEBUG_DUMP_PKGS *******
# Dump every package received by the server as hexdump format.
# ON  : Enable
# OFF : Disable
LIBOPEN62541_DEBUG_DUMP_PKG ??= "OFF"

# ********** PUBSUB *******
# Enable publish/subscribe
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_PUBSUB ??= "ON"

# ********** PUBSUB_ETH_UADP *******
# Enable publish/subscribe UADP over Ethernet
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_PUBSUB_ETH_UADP ??= "ON"

# ********** PUBSUB_ETH_RX_HW_TIMESTAMP *******
# Enable Enable getting Rx HW timestamp on Ethernet packet
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_PUBSUB_ETH_RX_HW_TIMESTAMP ??= "ON"


# ********** MALLOC_SINGLETON *******
# Use a global variable pointer for malloc (and free, ...) that can be switched at runtime
# ON  : Enable
# OFF : Disable
LIBOPEN62541_ENABLE_MALLOC_SINGLETON ??= "OFF"


EXTRA_OECMAKE:append:pn-libopen62541 = " \
	-DUA_ENABLE_MALLOC_SINGLETON=${LIBOPEN62541_ENABLE_MALLOC_SINGLETON} \
	-DUA_ENABLE_PUBSUB=${LIBOPEN62541_ENABLE_PUBSUB} \
	-DUA_ENABLE_PUBSUB_ETH_UADP=${LIBOPEN62541_ENABLE_PUBSUB_ETH_UADP} \
	-DUA_ENABLE_PUBSUB_ETH_RX_HW_TIMESTAMP=${LIBOPEN62541_ENABLE_PUBSUB_ETH_RX_HW_TIMESTAMP} \
	-DCMAKE_BUILD_TYPE=${LIBOPEN62541_BUILD_TYPE} \
	-DUA_LOGLEVEL=${LIBOPEN62541_LOGLEVE} \
	-DUA_MULTITHREADING=${LIBOPEN62541_MULTITHREADING} \
	-DUA_BUILD_UNIT_TESTS=${LIBOPEN62541_BUILD_UNIT_TESTS} \
	-DUA_ENABLE_SUBSCRIPTIONS=${LIBOPEN62541_ENABLE_SUBSCRIPTIONS} \
	-DUA_ENABLE_SUBSCRIPTIONS_EVENTS=${LIBOPEN62541_ENABLE_SUBSCRIPTIONS_EVENTS} \
	-DUA_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS=${LIBOPEN62541_ENABLE_SUBSCRIPTIONS_ALARMS_CONDITIONS} \
	-DUA_ENABLE_METHODCALLS=${LIBOPEN62541_ENABLE_METHODCALLS} \
	-DUA_ENABLE_PARSING=${LIBOPEN62541_ENABLE_PARSING} \
	-DUA_ENABLE_NODEMANAGEMENT=${LIBOPEN62541_ENABLE_NODEMANAGEMENT} \
	-DUA_ENABLE_AMALGAMATION=${LIBOPEN62541_ENABLE_AMALGAMATION} \
	-DUA_ENABLE_IMMUTABLE_NODES=${LIBOPEN62541_ENABLE_IMMUTABLE_NODES} \
	-DUA_ENABLE_COVERAGE=${LIBOPEN62541_ENABLE_COVERAGE} \
	-DUA_ENABLE_DISCOVERY=${LIBOPEN62541_ENABLE_DISCOVERY} \
	-DUA_ENABLE_DISCOVERY_MULTICAST=${LIBOPEN62541_ENABLE_DISCOVERY_MULTICAST} \
	-DUA_ENABLE_DISCOVERY_SEMAPHORE=${LIBOPEN62541_ENABLE_DISCOVERY_SEMAPHORE} \
	-DUA_NAMESPACE_ZERO=${LIBOPEN62541_NAMESPACE_ZERO} \
	-DUA_ENABLE_TYPEDESCRIPTION=${LIBOPEN62541_ENABLE_TYPEDESCRIPTION} \
	-DUA_ENABLE_STATUSCODE_DESCRIPTIONS=${LIBOPEN62541_ENABLE_STATUSCODE_DESCRIPTIONS} \
	-DUA_FILE_NS0=${LIBOPEN62541_ENABLE_FULL_NS0} \
	-DBUILD_SHARED_LIBS=${LIBOPEN62541_BUILD_SHARED_LIBS} \
	-DUA_BUILD_EXAMPLES=${LIBOPEN62541_BUILD_EXAMPLES} \
	-DUA_DEBUG=${LIBOPEN62541_BUILD_DEBUG} \
	-DUA_DEBUG_DUMP_PKGS=${LIBOPEN62541_DEBUG_DUMP_PKG} \
"
