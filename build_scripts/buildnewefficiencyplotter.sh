#!/usr/bin/bash

cd scripts
cd Efficiency

g++ -std=c++11 -O3 -march=native -w -Wall -Wl,--no-as-needed new_efficiency.cpp `root-config --libs ` -lMinuit -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o $scripts/efficiency_plotter

cd ../..