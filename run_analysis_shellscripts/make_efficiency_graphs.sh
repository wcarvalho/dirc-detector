#!/bin/bash

# source analysisSettings.sh
source setup_resultsfolders.sh
source current_files.sh

Time=$(timestamp)

$script_location/eff -p $cheat -r $rec -m $matchfalse_pre -f $fits_pre --momentum-slices settings/momentum --multiplicity-slices settings/multiplicity -c 2 --ptbins 15
addtime_movepdffiles $matchfalse_pre $Time $matchfalse_post
addtime_movepdffiles $fits_pre $Time $fits_post
