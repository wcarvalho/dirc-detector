#!/usr/bin/bash

cd scripts
cd print_photon_data

g++ -std=c++11 -O3 -march=native -w -Wall -Wl,--no-as-needed print_photon_data.cpp `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap`  `pkg-config --libs ReconstructionObjs` -o $scripts/print_photondata

cd ../..