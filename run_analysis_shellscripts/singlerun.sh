#!/bin/bash

source shell_functions.sh
source setup_datafolders.sh
rm_file $runOutput
touch $runOutput

if [ "$runGen" = true ] ; then
	source generate_particles.sh
fi
if [ "$runSim" = true ] ; then
	source generate_photons.sh
fi

if [ "$runSmear" = true ] ; then
	source smear_photons.sh
fi

# if [ "$runPixelate" = true ] ; then
# 	source pixelate_photons.sh
# fi

if [ "$runRec" = true ] ; then
	source reconstruct_particles.sh
fi

source ~/finish_sound.sh