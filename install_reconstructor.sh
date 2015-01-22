b=build
original='../..'
make="make -j 4 install"

cd reconstructor
pwd
cd $b
$make
cd $original