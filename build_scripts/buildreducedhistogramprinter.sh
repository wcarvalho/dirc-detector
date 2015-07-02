#!/usr/bin/env

cd scripts
cd HistogramPrinter

g++ -std=c++11 -O3 -g -march=native -w -Wall ReducedHistogramPrinter.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap`  `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o $scripts/print_reduced_histograms

cd ../..