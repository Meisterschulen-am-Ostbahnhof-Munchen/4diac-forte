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
# Enables an opc.ws:// endpoint alongside the plain opc.tcp:// one, one port
# above it. Requires open62541 built with UA_ENABLE_LWS (libwebsockets),
# which is not yet part of upstream open62541 as of writing; see
# https://github.com/open62541/open62541/pull/8259 and, on top of that,
# https://github.com/o6-automation/open62541-dev/pull/2. Until that lands,
# building with this option requires an open62541 with those changes applied.
option(FORTE_COM_OPC_UA_WEBSOCKET
        "Enable an OPC UA over WebSocket (opc.ws://) server endpoint. Requires open62541 built with UA_ENABLE_LWS." OFF
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

add_subdirectory(include)
add_subdirectory(src)

install(TARGETS forte-com-opc_ua EXPORT forte-export FILE_SET HEADERS)
