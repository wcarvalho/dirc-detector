#!/usr/bin/bash

cd scripts
cd Reconstruction_distribution

g++ -std=c++11 -O3 -march=native -w -Wall -Wl,--no-as-needed generate_reconstruction_theta_distribution.cpp `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/generate_theta_distribution

cd ../..