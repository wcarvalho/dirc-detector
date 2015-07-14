#!/usr/bin/env

directory=generator
source commands.sh
cmd="$configure=$MYLIBS --bindir=$MYAPPS/bin"
source execution.sh
