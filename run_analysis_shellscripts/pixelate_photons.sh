#!/bin/bash

source runSettings.sh
source shell_functions.sh

## FIXME: implement pixelate
# sim_pix=$smearfolder/$photons_file"_pixelated"$suffix
# echo "pixelate"
# 	$script_location/$scripts/"pixelate" -S$t/$sim_out_smeared -o$t/$sim_out_pix -T1
# sim=$sim_pix

echo "sim=$sim" >> $runOutput