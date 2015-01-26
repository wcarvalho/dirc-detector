#!/bin/bash

source current_files.sh

echo "generator -e $nevents -f $gen -c $parameters"
print_quiet_function "generator -e $nevents -f $gen -c $parameters"