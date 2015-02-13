#!/usr/bin/env

cd scripts
cd Efficiency

g++ -std=c++11 -O3 -march=native -w -Wall efficiency.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --cflags wilkasUtilityLib` -o ../eff

cd ../..