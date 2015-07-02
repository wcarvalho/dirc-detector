#!/usr/bin/env

cd scripts
cd embedParticles

g++ -std=c++11 -O3 -march=native -w -Wall embed_particles.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --libs SimObjs` -o $scripts/embed_particles

cd ../..