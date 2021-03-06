#!/usr/bin/bash

cd scripts
cd Photon#Discrepancy

g++ `LDFLAGS=-Wl,--no-as-needed` -O3 -march=native -w -Wall photonComparison.cpp `pkg-config --libs DircObjs` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs`  -o $scripts/photoncomp

cd ../..