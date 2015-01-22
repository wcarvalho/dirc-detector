#!/bin/bash

runphotoncomp=true
runsig=true
runEff=true

# print=true
quiet=true

runOutput="runOutput.sh"

main_directory="results"
script_location="scripts"

# folder names
pre="preprocess"
date=$(date +"%m_%e_%Y")
photon_comp="photon_comp_graphs"
matchfalse="matchfalse_graphs"
fits="fits"
delSeg_pt="delSeg_pt"

# folder paths
photon_comp=$main_directory/$photon_comp
matchfalse=$main_directory/$matchfalse
fits=$main_directory/$fits
delSeg_pt=$main_directory/$delSeg_pt

