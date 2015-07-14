#!/usr/bin/env

directory=simulator
source commands.sh
cmd="$configure=$MYLIBS --bindir=$MYAPPS/bin"
source execution.sh
