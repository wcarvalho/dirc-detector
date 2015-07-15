#!/usr/bin/bash

cd scripts
cd pt_sigma_comp

g++ -O3 -march=native -w -Wall -Wl,--no-as-needed delSig_pt.cpp `pkg-config --libs DircObjs` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/delSig_pt

cd ../..