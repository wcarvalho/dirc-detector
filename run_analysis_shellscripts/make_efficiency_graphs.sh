#!/bin/bash

# source analysisSettings.sh
source setup_resultsfolders.sh
source current_files.sh

Time=$(timestamp)

makefits="$script_location/eff -p $cheat -r $rec -m $matchfalse_pre -f $fits_pre --momentum-slices settings/momentum --multiplicity-slices settings/multiplicity -e 0 -e 8 -t 1.5"

makeplots="$script_location/eff -p $cheat -r $rec -m $matchfalse_pre -f $fits_pre --momentum-slices settings/momentum --multiplicity-slices settings/multiplicity -t 1.5 -c 2"

cmd=$makeplots

echo $cmd

$cmd
addtime_movepdffiles $matchfalse_pre $Time $matchfalse_post
addtime_movepdffiles $fits_pre $Time $fits_post
