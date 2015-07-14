#!/usr/bin/bash

cd scripts
cd FakeVsEfficiency

g++ -std=c++11 -O3 -march=native -w -Wall from_hists.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/generate_FakevsEff_fromHists

cd ../..