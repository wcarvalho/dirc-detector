#!/usr/bin/env

colorprint(){
	echo "\033[31m$1\033[00m"
}
colorprint "DIRC OBJECTS"
source install_dircobjects.sh
colorprint "GENERATOR"
source install_generator.sh
colorprint "SIMULATOR"
source install_simulator.sh
colorprint "RECONSTRUCTOR"
source install_reconstructor.sh
# source buildallscripts.sh