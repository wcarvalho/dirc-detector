#!/bin/bash

source setup_resultsfolders.sh
source current_files.sh

Time=$(timestamp)

$script_location/delSig_pt -S $cheat -R $rec -D $delSeg_pt_pre -p "settings/momentum" -a 40 -a 40
addtime_movepdffiles $delSeg_pt_pre $Time $delSeg_pt_post
