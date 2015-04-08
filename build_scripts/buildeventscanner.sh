#!/usr/bin/env

cd scripts
cd Information_LookUp

g++ -std=c++11 -O3 -march=native -w -Wall scan_events.cpp -rpath `root-config --libs ` -lMinuit -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o ../scan_events

cd ../..