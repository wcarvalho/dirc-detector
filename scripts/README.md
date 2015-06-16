##Available Scripts

|Script| Description|
| :---: | :--- |
| [Calibration][1] | |
| [Photon Angle Smearing][2] | |
| [Embdedding Particles][3] | |
| [Looking Up Particle Information][4] | |
| [Printing Histograms][5] | |
| [Plotting Efficiency and False-Positive Rates][6] | |
| [Plotting Efficiency vs. False-Positive Rate][7] | |


##Installation Instructions

Many of the scripts, rely on the "Templatized C++ Command Line Parser Library" ([TCLAP](http://tclap.sourceforge.net/)) library in order to parse command line options. In order to use these scripts, you must install this library.

#####Calibration
######calibration data generation
```
cd Calibrator
g++ -std=c++11 -O3 -march=native -w -Wall get_calibration_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o generate_calibration_data
```
######data calibration
```
cd Calibrator
g++ -std=c++11 -O3 -march=native -w -Wall calibrate_data.cpp `root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --cflags tclap` -o calibrate_data
```
#####Photon Angle Smearing
```
cd Smearer
g++ -std=c++11 -O3 -march=native -w -Wall Smearer.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o smear_photon_angles
```

#####Embdedding Particles
```
cd embedParticles
g++ -std=c++11 -O3 -march=native -w -Wall embed_particles.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` `pkg-config --libs SimObjs` -o ../embed_particles
```
#####Looking Up Particle Information

#####Printing Histograms

#####Plotting Efficiency and False-Positive Rates

#####Plotting Efficiency vs. False-Positive Rate


[1]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#calibration
[2]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#photon-angle-smearing
[3]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Embdedding-Particles
[4]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Looking-Up-Particle-Information
[5]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Printing-Histograms
[6]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Plotting-Efficiency-and-False-Positive-Rates
[7]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Plotting-Efficiency-vs.-False-Positive-Rate