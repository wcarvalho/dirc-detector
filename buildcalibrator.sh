#!/usr/bin/env

cd scripts
cd Calibrator

g++ -std=c++11 -O3 -march=native -w -Wall calibrate_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs wilkasUtilityLib` `pkg-config --cflags wilkasUtilityLib` -o ../calibrate_data

cd ../..