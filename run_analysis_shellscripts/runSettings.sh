#!/bin/bash

runGen=true
runSim=true
runSmear=true
runRec=true
#### not yet added
# runPixelate=true

# print=true
# quiet=true

parameters="settings/parameters.txt"

main_directory="data"
script_location="scripts"

## number of events
# nevents=5
nevents=1000
# nevents=2000

## number of particles
mparticles=1

## type of particle
# particle_type="Electrons"
particle_type="Pions"

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
runOutput="runOutput.sh"

setoutput(){
	echo $current_run/$1$suffix
}