#!/bin/bash

###########################
## Functions
missing_folder() {
	if [ ! -d $1 ]; then
		mkdir $1
	fi
}
missing_folder_set_variable(){
	missing_folder $1/$2
	echo $1/$2
}
rm_file(){
	if [ -e $1 ]; then
		rm $1
	fi
}
print_quiet_function(){
	if [ "$print" = true ] ; then
		$1 -v
	elif [ "$quiet" = true ] ; then
		$1 -q
	else
		$1
	fi
}
checkif_set(){
	if [ -e $1 ]; then
		echo $1
	else
		echo $2
	fi
}
timestamp() {
  date +"%H_%M"
}

timeAndDate() {
	#day_month_year_hour_min_am/pm
  date +"%m_%d_%y""%r"
}

addtime_movepdffiles(){
	newpath=$3
	for f in $1/*.pdf; do
		file=$(basename $f)
		oldpath=$(dirname $f)
		if [ "$(ls -A $oldpath)" ]; then
			mv $f $newpath/${file%.*}_$2.${file##*.}
		fi
	done
}