#!/usr/bin/env

cd scripts
cd HistogramFitPrinter

g++ -std=c++11 -O3 -march=native -w -Wall HistogramFitPrinter.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --libs ReconstructionObjs` -o ../printfits

cd ../..