#!/bin/bash

runGen=true
runSim=true
runSmear=true
runRec=true

# runPixelate=true

# print=true
quiet=true

## number of events
nevents=5
# nevents=1000
# nevents=2000

## number of particles
mparticles=1

## type of particle
# particle_type="Electrons"
particle_type="Pions"

main_directory="data"
current_run=$main_directory/$nevents"_events_"$mparticles"_particles"_$particle_type

## run options
smear=0.01
Case=2			# 1: lookuptable, 2: riemann sum

# folder names
scripts="scripts"
temp_scripts="temp_scripts"
smearfolder="smear"$smear
casefolder="case"$Case

# file names
particles_file="particles"
photons_file="photons"
cheat_file="cheat"
reconstruction_file="particle_reconstructions"
suffix=".root"

source shell_functions.sh
setoutput(){
	echo $current_run/$1$suffix
}
######################
## make directories
missing_folder $main_directory
missing_folder $current_run
smearfolder=$(missing_folder_set_variable $current_run $smearfolder)
######################

gen=$(setoutput $particles_file)
if [ "$runGen" = true ] ; then
	echo "generator"
	rm_file $gen
	print_quiet_function "generator -e $nevents -f $gen -c parameters.txt"
fi

cheat=$(setoutput $cheat_file)
sim=$(setoutput $photons_file)
if [ "$runSim" = true ] ; then
	echo "simulator"
	rm_file $sim
	print_quiet_function "simulator -i $gen -W $sim"
fi

sim_smeared=$smearfolder/$photons_file$suffix
if [ "$runSmear" = true ] ; then
	echo "smear"
	rm_file $sim_smeared
	./$scripts/smear -S$sim -o$sim_smeared -T$smear
fi
## if photons have been smeared, sim file = smeared file
sim=$(checkif_set $sim_smeared $sim)

## FIXME: implement pixelate
# sim_pix=$smearfolder/$photons_file"_pixelated"$suffix
# if [ "$runPixelate" = true ] ; then
# 	./$scripts/"pixelate" -S$t/$sim_out_smeared -o$t/$sim_out_pix -T1
# fi
# ## if photons have been pixelated, sim file = pixelated file
# sim=$(checkif_set $sim_pix $sim)

rec=$smearfolder/$reconstruction_file"_case"$Case$suffix
if [ "$runRec" = true ] ; then
	echo "reconstructor"
	rm_file $rec 
	print_quiet_function "reconstructor -i $sim -e$Case -W $rec"
fi

rm_file runOutput
touch runOutput
echo "gen=$gen" >> runOutput
echo "cheat=$cheat" >> runOutput
echo "sim=$sim" >> runOutput
echo "rec=$rec" >> runOutput

sh ~/finish_sound.sh