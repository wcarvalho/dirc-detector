#!/usr/bin/bash

cd scripts
cd Smearer

g++ -Wl,--no-as-needed -std=c++11 -O3 -march=native -w -Wall Smearer.cpp `pkg-config --libs DircObjs``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/smear

cd ../..