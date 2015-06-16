##Available Scripts

|Script| Description| Installation|
| :---: | :--- | :--- |
| Calibration | | [link][1] |
| Photon Angle Smearing | | [link][2] |
| Embdedding Particles | | [link][3] |
| Looking Up Particle Information | | [link][4] |
| Printing Histograms | | [link][5] |
| Plotting Efficiency and False-Positive Rates | | [link][6] |
| Plotting Efficiency vs. False-Positive Rate | | [link][7] |


##Installation Instructions

Many of the scripts, rely on the "Templatized C++ Command Line Parser Library" ([TCLAP](http://tclap.sourceforge.net/)) library in order to parse command line options. In order to use these scripts, you must install this library.

#####Calibrating Data
######generating calibration data
```
cd Scripts
cd Calibrator
g++ -std=c++11 -O3 -march=native -w -Wall get_calibration_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o generate_calibration_data
```
######calibrating data
```
cd Scripts
cd Calibrator
g++ -std=c++11 -O3 -march=native -w -Wall calibrate_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o calibrate_data
```
#####Smearing Angles
```
cd Scripts
cd Smearer
g++ -std=c++11 -O3 -march=native -w -Wall Smearer.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o smear_photon_angles
```

#####Embdedding Particles
```
cd Scripts
cd embedParticles
g++ -std=c++11 -O3 -march=native -w -Wall embed_particles.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --libs SimObjs` -o embed_particles
```
#####Looking Up Particle Information
```
cd Scripts
cd Information_LookUp
g++ -std=c++11 -O3 -march=native -w -Wall info_lookup.cpp -rpath `root-config --libs ` -lMinuit -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap`  `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o lookup_particle_info
```

#####Printing Histograms
######Printing Full Histograms
```
cd scripts
cd HistogramPrinter
g++ -std=c++11 -O3 -march=native -w -Wall HistogramPrinter.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap`  `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o print_histograms
```
######Printing Indexed Histograms
```
cd Scripts
cd HistogramPrinter
g++ -std=c++11 -O3 -g -march=native -w -Wall ReducedHistogramPrinter.cpp -rpath `root-config --libs ` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap`  `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o print_reduced_histograms
```
#####Plotting Efficiency and False-Positive Rates
```
cd scripts
cd Efficiency
g++ -std=c++11 -O3 -march=native -w -Wall new_efficiency.cpp -rpath `root-config --libs ` -lMinuit -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` `pkg-config --libs ReconstructionObjs` `pkg-config --libs SimObjs` -o plot_efficiency
```
#####Plotting Efficiency vs. False-Positive Rate
```
cd scripts
cd FakeVsEfficiency
g++ -std=c++11 -O3 -march=native -w -Wall fakevsefficiency.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o plot_fake_vs_efficiency
```

[1]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#calibrating-data
[2]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#smearing-angles
[3]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#embdedding-particles
[4]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#looking-up-particle-information
[5]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#printing-histograms
[6]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#plotting-efficiency-and-false-positive-rates
[7]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#plotting-efficiency-vs-false-positive-rate