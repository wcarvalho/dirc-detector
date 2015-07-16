#!/usr/bin/bash

cd scripts
cd Calibrator

g++ `LDFLAGS=-Wl,--no-as-needed` -std=c++11 -O3 -march=native -w -Wall get_calibration_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o $scripts/generateCalibrationData

cd ../..