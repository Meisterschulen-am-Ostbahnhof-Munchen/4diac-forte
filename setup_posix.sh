#!/bin/bash
echo "----------------------------------------------------------------------------"
echo " Automatically set up development environment for POSIX-platform"
echo "----------------------------------------------------------------------------"
echo ""
echo " Includes 64bit-datatypes, float-datatypes, Ethernet-Interface,"
echo " ASN1-encoding, ..."
echo ""
echo " To include tests set directories for boost-test-framework and "
echo " set FORTE_TESTS-option to 'ON'"
echo ""
echo "----------------------------------------------------------------------------"

export forte_bin_dir="bin/posix"

#set to boost-include directory
export forte_boost_test_inc_dirs=""
#set to boost-library directory
export forte_boost_test_lib_dirs=""

if [ ! -d "$forte_bin_dir" ]; then
  mkdir -p "$forte_bin_dir"
fi

if [ -d "$forte_bin_dir" ]; then
  
  echo "For building forte go to $forte_bin_dir and execute \"ninja\""
  echo "forte can be found at ${forte_bin_dir}/src"
  echo "forte_tests can be found at ${forte_bin_dir}/tests"
  
  
  cd "./$forte_bin_dir"
  
cmake -G "Ninja" \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
    -DCMAKE_INSTALL_PREFIX="/home/runner/work/4diac-forte/4diac-forte/install/posix-systemtests" \
    -DCMAKE_INSTALL_RPATH='$ORIGIN/../lib' \
    -DCMAKE_INSTALL_RPATH_USE_LINK_PATH=ON \
    -DFORTE_ARCHITECTURE=Posix \
    -DFORTE_EventChainExternalEventListSize=512 \
    -DFORTE_EventChainEventListSize=1024 \
    -DFORTE_IO=ON \
    -DFORTE_LOGLEVEL=LOGDEBUG \
    -DFORTE_COM_ETH=ON \
    -DFORTE_COM_FBDK=ON \
    -DFORTE_COM_HTTP=ON \
    -DFORTE_COM_LOCAL=ON \
    -DFORTE_COM_RAW=ON \
    -DFORTE_COM_SER=ON \
    -DFORTE_COM_STRUCT_MEMBER=ON \
    -DFORTE_IO=ON \
    -DFORTE_MODULE_CONVERT=ON \
    -DFORTE_MODULE_IEC61131=ON \
    -DFORTE_MODULE_RECONFIGURATION=ON \
    -DFORTE_MODULE_RT_Events=ON \
    -DFORTE_MODULE_SIGNALPROCESSING=ON \
    -DFORTE_MODULE_UTILS=ON \
    -DFORTE_SYSTEM_TESTS=ON \
    -DFORTE_TESTS=ON \
    -DFORTE_TESTS_INC_DIRS=${forte_boost_test_inc_dirs} \
    -DFORTE_TESTS_LINK_DIRS=${forte_boost_test_inc_dirs} \
    -DFORTE_TEST_SANITIZE=ON \
    -DCMAKE_CXX_FLAGS="-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer" \
    -DCMAKE_C_FLAGS="-fsanitize=address -fsanitize=undefined -fno-omit-frame-pointer" \
    -DCMAKE_EXE_LINKER_FLAGS="-fsanitize=address -fsanitize=undefined" \
    -DCMAKE_SHARED_LINKER_FLAGS="-fsanitize=address -fsanitize=undefined" \
    ../../
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
