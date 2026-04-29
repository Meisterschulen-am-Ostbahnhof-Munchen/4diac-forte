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
    -DFORTE_ARCHITECTURE=Posix \
    -DFORTE_EventChainExternalEventListSize=512 \
    -DFORTE_EventChainEventListSize=1024 \
    -DFORTE_IO=ON \
    -DFORTE_LOGLEVEL=LOGDEBUG \
    -DFORTE_COM_ETH=ON \
    -DFORTE_COM_FBDK=ON \
    -DFORTE_COM_LOCAL=ON \
    -DFORTE_TESTS=ON \
    -DFORTE_SYSTEM_TESTS=ON \
    -DFORTE_FILEIO_RAMDISK=ON \
    -DFORTE_MODULE_CONVERT=ON \
    -DFORTE_MODULE_RT_Events=ON \
    -DFORTE_MODULE_IEC61131=ON \
    -DFORTE_MODULE_CONVERT=ON \
    -DFORTE_MODULE_UTILS=ON \
    ../../
else
  echo "unable to create ${forte_bin_dir}"
  exit 1
fi
