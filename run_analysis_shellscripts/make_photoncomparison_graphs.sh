#!/bin/bash

source analysisSettings.sh
source shell_functions.sh
source setup_resultsfolders.sh
source $runOutput

Time=$(timestamp)

$script_location/photoncomp -S$cheat -R$rec -g$photon_comp_pre
addtime_movepdffiles $photon_comp_pre $Time $photon_comp_post