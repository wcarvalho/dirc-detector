#!/usr/bin/env

cd scripts
cd HistogramPrinter

g++ -std=c++11 -O3 -g -march=native -w -Wall CombinationPrinter.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --libs ReconstructionObjs` -o ../print_scatter_projection_combinations

cd ../..