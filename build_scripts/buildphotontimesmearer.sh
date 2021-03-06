#!/usr/bin/bash

cd scripts
cd SmearPhotonTime

g++ `LDFLAGS=-Wl,--no-as-needed` -O3 -march=native -w -Wall smear_photon_time.cpp `pkg-config --libs DircObjs``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o $scripts/smear_photon_time

cd ../..