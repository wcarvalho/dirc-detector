#!/usr/bin/env

cd scripts
cd SmearPhotonTime

g++ -O3 -march=native -w -Wall smear_photon_time.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/smear_photon_time

cd ../..