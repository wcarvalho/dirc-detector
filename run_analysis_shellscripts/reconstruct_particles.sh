#!/bin/bash

source current_files.sh

echo "reconstructor"
print_quiet_function "reconstructor -i $sim -e$Case -W $rec"