#!/usr/bin/bash

directory=generator
source commands.sh
cmd="$configure=$MYLIBS --bindir=$MYAPPS"
source execution.sh
