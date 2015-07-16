#!/usr/bin/bash

cd scripts
cd Pixelater

g++ `LDFLAGS=-Wl,--no-as-needed` -O3 -march=native -w -Wall pixelate_photons.cpp `pkg-config --libs DircObjs``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/pixelate

cd ../..