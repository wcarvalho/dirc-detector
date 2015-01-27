#!/usr/bin/env

cd scripts
cd Photon#Discrepancy

g++ -O3 -march=native -w -Wall photonComparison.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --libs wilkasUtilityLib` -o ../photoncomp

cd ../..