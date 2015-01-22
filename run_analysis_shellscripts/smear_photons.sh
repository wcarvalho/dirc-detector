#!/bin/bash

source current_files.sh

echo "smear"
$script_location/smear -S$sim -o$sim_smeared -T$smear