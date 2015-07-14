#!/usr/bin/bash

directory=simulator
source commands.sh
cmd="$configure=$MYLIBS --bindir=$MYAPPS"
source execution.sh
