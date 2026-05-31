#!/bin/bash
echo "----------------------------------------------------------------------------"


export forte_bin_dir="bin/posix"

#rm old folder
rm -r "./$forte_bin_dir"

#call setup_posix.sh
./setup_posix.sh

#goto new Directory
cd "./$forte_bin_dir"

#make
ninja -j 24


echo "----------------------------------------------------------------------------"
echo " DONE. to re-ninja just type:"
echo "cd ./$forte_bin_dir"
echo "ninja -j 24" 
echo "----------------------------------------------------------------------------"

