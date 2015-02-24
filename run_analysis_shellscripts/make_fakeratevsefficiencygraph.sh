#!/bin/bash

source setup_resultsfolders.sh
source current_files.sh

basecommand="$script_location/plotfakevsefficiency -e -f -o"

cmd="$basecommand"

echo $cmd

$cmd

#calibration graphs
addtime_movepdffiles "$fakevsmatch_pre" "$analysis_description" "$fakevsmatch_post"