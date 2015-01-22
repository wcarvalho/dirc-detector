b=build
original='../..'
make="make -j 4"

cd simulator
pwd
cd $b
$make
cd $original
