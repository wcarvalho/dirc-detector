#!/usr/bin/bash

cd scripts
cd FakeVsEfficiency

g++ `LDFLAGS=-Wl,--no-as-needed` -std=c++11 -O3 -march=native -w -Wall fakevsefficiency.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/plotfakevsefficiency

cd ../..