#!/usr/bin/bash

cd scripts
cd Reconstruction_distribution

g++ `LDFLAGS=-Wl,--no-as-needed` -std=c++11 -O3 -march=native -w -Wall generate_reconstruction_theta_distribution.cpp `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/generate_theta_distribution

cd ../..