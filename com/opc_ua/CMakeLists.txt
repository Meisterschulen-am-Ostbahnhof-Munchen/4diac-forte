# *******************************************************************************************

# Copyright (c) 2015-2016 Florian Froschermeier
# <florian.froschermeier@tum.de>, fortiss GmbH This program and the
# accompanying materials are made available under the terms of the Eclipse
# Public License 2.0 which is available at
# http://www.eclipse.org/legal/epl-2.0.
#
# SPDX-License-Identifier: EPL-2.0
#
# Contributors: Florian Froschermeier - initial integration of OPC Unified
# Architecture into FORTE Stefan Profanter - refactoring and adaption to new
# concept
# ******************************************************************************************

# ############################################################################
# OPC Unified Architecture (UA) integration. For integration the open-source
# OPC-UA communication stack/library from (open62541 - http://open62541.org/)
# was used.
# ############################################################################

# option to enable OPC UA with cmake
option(FORTE_COM_OPC_UA "Enable OPC UA communication layer" OFF)
if (NOT FORTE_COM_OPC_UA)
    return()
endif ()

# keep in sync with forte-config.cmake.in and dependencies.spdx
find_package(open62541 1.5.4 REQUIRED)

set(FORTE_COM_OPC_UA_PORT
        4840
        CACHE STRING "Port where the OPC_UA server will listen"
)
set(FORTE_COM_OPC_UA_SERVER_PUB_INTERVAL
        100.0
        CACHE STRING "OPC UA Server publishing interval in ms"
)
set(FORTE_COM_OPC_UA_CLIENT_PUB_INTERVAL
        100.0
        CACHE STRING
        "OPC UA Client requested publishing interval via OPC UA data subscription in ms"
)
set(FORTE_COM_OPC_UA_SERVER_MAX_ITERATION_INTERVAL
        10
        CACHE STRING
        "The maximum time in OPC UA server will wait to iterate again"
)

option(FORTE_COM_OPC_UA_MULTICAST
        "Enable multicast support for OPC UA and registering with LDS" OFF
)
set(FORTE_COM_OPC_UA_CUSTOM_HOSTNAME
        CACHE STRING
        "Custom hostname which is used for the OPC UA app name and app uri"
)

# OPC UA Event Layer
option(FORTE_COM_OPC_UA_Events "Enable OPC UA Event Support" OFF)
option(FORTE_COM_OPC_UA_AC "Enable OPC UA A&C Support" OFF)

add_library(forte-com-opc_ua)
target_link_libraries(forte-com-opc_ua PUBLIC forte-core open62541::open62541)
target_link_libraries(forte PUBLIC $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,forte-com-opc_ua,$<LINK_LIBRARY:WHOLE_ARCHIVE,forte-com-opc_ua>>)

# Only compiled in for systemtest builds -- gates test-only instrumentation (e.g. the artificial
# startup delay hook in COPC_UA_Local_Handler::run(), used by OPCUA_LocalHandlerStartServerRace)
# out of production/release builds entirely, rather than relying on the hook being a no-op at
# runtime. Must be PUBLIC, not PRIVATE: opcua_local_handler.h is also included by other targets
# that link against forte-com-opc_ua (e.g. forte-system's OPCUA_MGR.cpp, which needs the full
# COPC_UA_Local_Handler definition). If those consumers were compiled without this definition
# while forte-com-opc_ua's own sources have it, COPC_UA_Local_Handler would have two different
# sizes/layouts across translation units -- a silent ODR violation that corrupts memory instead
# of failing to build.
target_compile_definitions(forte-com-opc_ua PUBLIC $<$<BOOL:${FORTE_SYSTEM_TESTS}>:FORTE_COM_OPC_UA_TEST_HOOKS>)

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-com-opc_ua EXPORT forte-export FILE_SET HEADERS)
