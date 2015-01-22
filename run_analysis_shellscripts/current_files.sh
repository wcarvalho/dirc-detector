#!/bin/bash

source runSettings.sh
source shell_functions.sh

gen=$(setoutput $particles_file)
cheat=$(setoutput $cheat_file)
sim=$(setoutput $photons_file)

smearfolder=$(missing_folder_set_variable $current_run $smearfolder)
sim_smeared=$smearfolder/$photons_file$suffix

## if photons have been smeared, sim file = smeared file
sim=$(checkif_set $sim_smeared $sim)

rec=$smearfolder/$reconstruction_file"_case"$Case$suffix