b=build
original='../..'
make="../configure --prefix=$MYAPPS"

cd generator
pwd
cd $b
$make
cd $original

cd simulator
pwd
cd $b
$make
cd $original

cd reconstructor
pwd
cd $b
$make
cd $original

cd histmaker
pwd
cd $b
$make
cd $original

cd fitter
pwd
cd $b
$make
cd $original