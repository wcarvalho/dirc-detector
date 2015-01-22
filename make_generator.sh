b=build
original='../..'
make="make -j 4"

cd generator
pwd
cd $b
$make
cd $original
