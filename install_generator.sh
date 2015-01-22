b=build
original='../..'
make="make -j 4 install"

cd generator
pwd
cd $b
$make
cd $original
