#!/bin/bash
echo "----------------------------------------------------------------------------"


export forte_bin_dir="bin/posix"

#rm old folder
rm -r "./$forte_bin_dir"

#call setup_esp32c3.sh
./setup_posix.sh

#goto new Directory
cd "./$forte_bin_dir"

#ninja
ninja


echo "----------------------------------------------------------------------------"
echo " DONE. to re-ninja just type:"
echo "cd ./$forte_bin_dir"
echo "ninja -j" 
echo "----------------------------------------------------------------------------"

