#!/usr/bin/bash

colorprint(){
	echo "\033[31m$1\033[00m"
}
colorprint "DIRC OBJECTS"
source install_dircobjects.sh
colorprint "SIMULATOR"
source install_simulator.sh
colorprint "GENERATOR"
source install_generator.sh
colorprint "RECONSTRUCTOR"
source install_reconstructor.sh
# source buildallscripts.sh