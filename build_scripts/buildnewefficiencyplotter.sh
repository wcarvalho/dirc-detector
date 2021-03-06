#!/usr/bin/bash

cd scripts
cd Efficiency

g++ `LDFLAGS=-Wl,--no-as-needed` -std=c++11 -O3 -march=native -w -Wall new_efficiency.cpp `root-config --libs ` -lMinuit -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o $scripts/efficiency_plotter

cd ../..