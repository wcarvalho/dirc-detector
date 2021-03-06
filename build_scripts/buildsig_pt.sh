#!/usr/bin/bash

cd scripts
cd pt_sigma_comp

g++ `LDFLAGS=-Wl,--no-as-needed` -O3 -march=native -w -Wall delSig_pt.cpp `pkg-config --libs DircObjs` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/delSig_pt

cd ../..