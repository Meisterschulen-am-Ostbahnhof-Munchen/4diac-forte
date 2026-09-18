#*******************************************************************************
# * Copyright (c) 2019 fortiss GmbH
# * All rights reserved. This program and the accompanying materials
# * are made available under the terms of the Eclipse Public License v1.0
# * which accompanies this distribution, and is available at
# * http://www.eclipse.org/legal/epl-v10.html
# *
# * Contributors:
# *   Jose Cabral  - initial API and implementation and/or initial documentation
# *******************************************************************************/

forte_add_systemtest_soft(OPCUA_WrongParams wrongParameters.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_WrongParams PROPERTIES FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|==ERROR|OPC UA LOCAL") #Local server shouldn't start
 
forte_add_systemtest_hard(OPCUA_LocalFirstRead localFirstRead.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_systemtest_hard(OPCUA_LocalFirstWrite localFirstWrite.fboot 10)

# Reproduces a bug in COPC_UA_Local_Handler::startServer(): isAlive() becomes true the instant
# the background run() thread is *scheduled*, long before UA_Server_run_startup() actually
# finishes (real startup can take hundreds of ms on embedded targets, dominated by loading the
# full standard nodeset). Any caller of startServer() that arrives while that startup is still in
# progress sees isAlive()==true, skips the intended wait, finds mUaServer still null, and fails
# permanently -- there is no retry anywhere in the call chain.
#
# This is reproducible even using the officially documented "-d OPCUA_DEV" device (see
# doc/tutorials/opcuadeployment.adoc), whose whole purpose is to start the local OPC UA server
# eagerly, before any resource/FB exists, specifically to avoid this class of problem:
# OPCUA_MGR::initialize() (called from OPCUA_DEV::startDevice()) calls enableHandler() itself,
# but that call does not block bootfile/resource processing, which proceeds on a separate thread
# regardless of whether the eager startServer() call has actually finished -- so the intended
# safeguard does not close the gap either. On a fast POSIX host the real startup normally
# finishes in well under 1ms, so the gap is too narrow to hit in practice without slowing it
# down. Both env vars below are test-only (compiled out entirely unless FORTE_SYSTEM_TESTS is
# enabled, no effect otherwise): FORTE_COM_OPC_UA_TEST_STARTUP_WAIT_FOR_ATTEMPTS makes run() hold
# startup until that many initializeAction() attempts have actually been observed (16, matching
# the 16 PUBLISH_1 instances below), rather than guessing a fixed wall-clock delay that could pass
# on an unfixed implementation if the host happens to be slow to even reach bootfile processing.
# FORTE_COM_OPC_UA_TEST_STARTUP_DELAY_MS is a safety ceiling for the (expected, harmless) case
# where the resource's single executor thread happens to dispatch a PUBLISH_1 instance that wins
# the race to become the very first startServer() caller: since that caller then blocks holding
# the same thread, later PUBLISH_1 instances in the fan-out never run and the count can get stuck
# below 16 -- keep this small (not a generous many-seconds value), since the test's declared
# TIMEOUT above must comfortably cover this ceiling plus the fboot's own Delay_Check/Delay_End/
# STEST_END sequence. See https://github.com/eclipse-4diac/4diac-forte/issues/1053
#
# NOTE: using "-d OPCUA_DEV" here exposes a separate, pre-existing memory leak in
# OPCUA_MGR::initialize() (confirmed present with or without this fix, see
# https://github.com/eclipse-4diac/4diac-forte/issues/1054) that no other systemtest hits
# because none of them use the OPCUA_DEV device. It is unrelated to this test's actual purpose
# (verifying startServer()'s readiness handling), so it is suppressed narrowly, by function name,
# via opcua_dev_known_leak.lsan-suppressions -- this only silences that specific known leak, not
# leak detection for the process, so a genuinely new leak is still caught. ASAN_OPTIONS below
# forces slow/precise unwinding (the default fast unwinder only resolves 2 stack frames on this
# build, too shallow for the suppression to match its FORTE-side frame).
file(TO_NATIVE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/opcuaLocalHandlerStartServerRace.fboot" opcua_race_bootfile)
add_test(NAME OPCUA_LocalHandlerStartServerRace
         COMMAND $<TARGET_FILE:forte_systemtest> -d OPCUA_DEV -f "${opcua_race_bootfile}")
set_tests_properties(OPCUA_LocalHandlerStartServerRace PROPERTIES
                      TIMEOUT 10
                      FAIL_REGULAR_EXPRESSION "TEST_CONDITION_FAILED|ERROR: T|==ERROR"
                      ENVIRONMENT "FORTE_COM_OPC_UA_TEST_STARTUP_DELAY_MS=500;FORTE_COM_OPC_UA_TEST_STARTUP_WAIT_FOR_ATTEMPTS=16;LSAN_OPTIONS=suppressions=${CMAKE_CURRENT_SOURCE_DIR}/opcua_dev_known_leak.lsan-suppressions;ASAN_OPTIONS=fast_unwind_on_malloc=0:malloc_context_size=30")
# Deliberately RED on this branch alone (see the fix in the stacked PR). WILL_FAIL (the pattern
# Test_Hard_Fail_Alaways_Inverted/Test_Soft_Fail_Alaways_Inverted use above) only inverts a
# nonzero *exit code* -- it has no effect here, since this failure is detected via
# FAIL_REGULAR_EXPRESSION matching TEST_CONDITION_FAILED in the output while the process itself
# still exits 0, and CTest treats a FAIL_REGULAR_EXPRESSION match as a failure unconditionally.
# DISABLED keeps develop's CI green if this PR is ever merged before its stacked fix PR instead:
# the test is skipped rather than run-and-failed. Removed by the stacked fix PR, where this test
# is expected to actually run and pass.
SET_TESTS_PROPERTIES(OPCUA_LocalHandlerStartServerRace PROPERTIES DISABLED TRUE)

forte_add_systemtest_hard(OPCUA_LocalMethdodsCreate localMethodsCreate.fboot 10)

SET_TESTS_PROPERTIES(OPCUA_LocalFirstRead PROPERTIES FAIL_REGULAR_EXPRESSION "ERROR: T|==ERROR|OPC UA REMOTE") #remote handler shouldn't start

forte_add_multi_systemtests(OPCUA_WorkingClientServer 60 0 clientWorking.fboot " " serverWorking.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_WorkingClientNoSubsServer 60 0 clientWorkingNoSubs.fboot " " serverWorking.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_FailingClientServer 60 0 failingClient.fboot " " failingServer.fboot " ")

forte_add_multi_systemtests(OPCUA_DelayedWorking 120 0 delayedClient.fboot " " delayedServer.fboot " ")

forte_add_multi_systemtests(OPCUA_TypesWorking 60 0 typesClient.fboot " " typesServer.fboot " ") #This could be hard, but connections errors sometimes occur which don't affect the TEST_CONDITIONS

forte_add_multi_systemtests(OPCUA_FailActions 60 0 clientFailAction.fboot " " localFailAction.fboot " ")


#Config files for clients

forte_add_multi_systemtests(OPCUA_ClientConfigFailNonExistingFile 30 0 clientAuthFail.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/NonExistingFile.forte" serverForAuth.fboot " " )

forte_add_multi_systemtests(OPCUA_ClientConfigFailNoEqualSignal1 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal1.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailNoEqualSignal2 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoEqualSignal2.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal1 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal1.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal2 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal2.forte" serverForAuth.fboot " ")

forte_add_multi_systemtests(OPCUA_ClientConfigFailTwoEqualSignal3 30 0 clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failTwoEqualSignal3.forte" serverForAuth.fboot " ")

# deactivated as not correctly working with open62541 v1.3.4
# forte_add_multi_systemtests(OPCUA_ClientConfigSuccessNoEndpoint 30 0 clientAuthSuccess.fboot "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNoEndpoint.forte" serverForAuth.fboot " ")

# deactivated as not correctly working with open62541 v1.3.4
# forte_add_multi_systemtests(OPCUA_ClientConfigSuccessNonMatchEndpointFirst 30 0 clientAuthSuccess.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/successNonMatchEndpointFirst.forte" serverForAuth.fboot " ")

if(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)
  forte_add_multi_systemtests(OPCUA_ClientConfigFailFailNoCertificateFile clientAuthFail.fboot  "-oc ${CMAKE_CURRENT_SOURCE_DIR}/clientsConfigFiles/failNoCertificateFile.forte" serverForAuth.fboot " ")
endif(FORTE_COM_OPC_UA_ENCRYPTION AND FORTE_COM_OPC_UA_MASTER_BRANCH)

# deactivated as not currently working with FB layout refactor
# forte_add_multi_systemtests(OPCUA_DoubleResourceDeadlock 30 0 doubleResourceDeadlockClient.fboot " " doubleResourceDeadlockServer.fboot " ") # There's no test in these systems. It only tests that if two resources try to start the local/remote thread at the same time, there won't be a deadlock, thus finishing the test without a timeout

forte_add_multi_systemtests(OPCUA_FastWrite 200 0 fastWriteClient.fboot " " fastWriteServer.fboot " ") # There's no test in these systems. It only tests quick accessing the server with a client subscribed to it, and there won't be any problem, thus finishing the test without a timeout

add_subdirectory(namespace)
