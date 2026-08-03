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

if (FORTE_COM_OPC_UA_INCLUDE_DIR)
    get_filename_component(FORTE_COM_OPC_UA_INCLUDE_DIR_ABS "${FORTE_COM_OPC_UA_INCLUDE_DIR}" ABSOLUTE)
    if (NOT TARGET open62541::open62541)
        # FORTE_COM_OPC_UA_INCLUDE_DIR points at an amalgamated open62541 build
        # (open62541.c/.h, no compiled library yet): on ESP-IDF targets that
        # source is compiled and linked one level up, as the outer project's
        # own "open62541" component, so forte only ever needs the headers here
        # to type-check against - never a real library to link.
        add_library(open62541 INTERFACE)
        add_library(open62541::open62541 ALIAS open62541)
        target_include_directories(open62541 INTERFACE
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/src_generated>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/include>
            $<BUILD_INTERFACE:${FORTE_COM_OPC_UA_INCLUDE_DIR_ABS}/../open62541/plugins/include>
        )
        install(TARGETS open62541 EXPORT forte-export)
    endif ()
else ()
    find_package(open62541 1.5.4 REQUIRED)
endif ()

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
# Enables opc.ws:// and opc.wss:// endpoints alongside the plain opc.tcp:// endpoint,
# using the ports immediately above it (opc.ws:// on port+1 and opc.wss:// on port+2).
# On platforms built with UA_ENABLE_LWS (libwebsockets, POSIX-only), open62541
# provides the "websocket" ConnectionManager itself. On platforms without
# libwebsockets (RTOS targets such as FreeRTOS/Zephyr), that ConnectionManager
# has to be registered by the platform's own code instead.
# sourcery skip: javascript.lang.security.detect-insecure-websocket
option(FORTE_COM_OPC_UA_WEBSOCKET
        "Enable OPC UA over WebSocket server endpoints." OFF
)

if (FORTE_COM_OPC_UA_WEBSOCKET)
    # Intentional opc.ws:// support (builder-configurable, off when INSECURE=OFF)
    # sourcery skip: javascript.lang.security.detect-insecure-websocket
    option(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE
            "Enable unencrypted OPC UA over WebSocket (opc.ws://) endpoint" OFF
    )
    option(FORTE_COM_OPC_UA_WEBSOCKET_SECURE
            "Enable secure OPC UA over WebSocket (opc.wss://) endpoint" ON
    )
    if (NOT FORTE_COM_OPC_UA_WEBSOCKET_INSECURE AND NOT FORTE_COM_OPC_UA_WEBSOCKET_SECURE)
        message(FATAL_ERROR "FORTE_COM_OPC_UA_WEBSOCKET is enabled, but neither FORTE_COM_OPC_UA_WEBSOCKET_INSECURE nor FORTE_COM_OPC_UA_WEBSOCKET_SECURE is selected.")
    endif ()
else ()
    set(FORTE_COM_OPC_UA_WEBSOCKET_INSECURE OFF)
    set(FORTE_COM_OPC_UA_WEBSOCKET_SECURE OFF)
endif ()
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
