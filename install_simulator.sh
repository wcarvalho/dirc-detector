b=build
original='../..'
make="make -j 4 install"

cd simulator
pwd
cd $b
$make
cd $original
