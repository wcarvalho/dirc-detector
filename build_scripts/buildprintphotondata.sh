#!/usr/bin/env

cd scripts
cd print_photon_data

g++ -std=c++11 -O3 -march=native -w -Wall print_photon_data.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --libs ReconstructionObjs` -o ../print_photondata

cd ../..