#!/usr/bin/bash

cd scripts
cd Smearer

g++ -std=c++11 -O3 -march=native -w -Wall Smearer.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/smear

cd ../..