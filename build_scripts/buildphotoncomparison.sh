#!/usr/bin/bash

cd scripts
cd Photon#Discrepancy

g++ -O3 -march=native -w -Wall -Wl,--no-as-needed photonComparison.cpp `pkg-config --libs DircObjs` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs`  -o $scripts/photoncomp

cd ../..