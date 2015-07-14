#!/usr/bin/bash

cd scripts
cd pt_sigma_comp

g++ -O3 -march=native -w -Wall delSig_pt.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'` `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/delSig_pt

cd ../..