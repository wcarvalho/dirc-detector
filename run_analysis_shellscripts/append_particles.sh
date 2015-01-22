#!/bin/bash

source generate_particles

source runSettings.sh
source shell_functions.sh
source $runOutput

cheat=$(setoutput $cheat_file)
sim=$(setoutput $photons_file)
echo "simulator"
print_quiet_function "simulator -A -i $gen -W $sim"

echo "cheat=$cheat" >> $runOutput
echo "sim=$sim" >> $runOutput