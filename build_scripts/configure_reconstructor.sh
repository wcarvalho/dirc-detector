#!/usr/bin/env

directory=reconstructor
source commands.sh
cmd="$configure=$MYLIBS --bindir=$MYAPPS/bin"
# echo $cmd
source execution.sh
