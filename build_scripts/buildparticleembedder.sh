#!/usr/bin/bash

cd scripts
cd embedParticles

g++ -std=c++11 -O3 -march=native -w -Wall -Wl,--no-as-needed embed_particles.cpp `pkg-config --libs DircObjs``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --libs SimObjs` -o $scripts/embed_particles

cd ../..