#!/usr/bin/env

cd scripts
cd Pixelater

g++ -O3 -march=native -w -Wall pixelate_photons.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/pixelate

cd ../..