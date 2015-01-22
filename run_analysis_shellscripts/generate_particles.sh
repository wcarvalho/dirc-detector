#!/bin/bash

source current_files.sh

echo "generator"
print_quiet_function "generator -e $nevents -f $gen -c $parameters"