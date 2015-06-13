##Installation Instructions

Once you have cloned the master directory from git, you can

|Script| Description|
| :---: | :--- |
| [Calibration][1] | |
| [Photon Angle Smearing][2] | |
| [Embdedding Particles][3] | |
| [Looking Up Particle Information][4] | |
| [Printing Histograms][5] | |
| [Plotting Efficiency and False-Positive Rates][6] | |
| [Plotting Efficiency vs. False-Positive Rate][7] | |



####Calibration

####Photon Angle Smearing

####Embdedding Particles

####Looking Up Particle Information

####Printing Histograms

####Plotting Efficiency and False-Positive Rates

####Plotting Efficiency vs. False-Positive Rate


```
g++ -std=c++11 -O3 -march=native -w -Wall Smearer.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o ../smear
```


[1]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#calibration
[2]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#photon-angle-smearing
[3]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Embdedding-Particles
[4]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Looking-Up-Particle-Information
[5]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Printing-Histograms
[6]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Plotting-Efficiency-and-False-Positive-Rates
[7]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts#Plotting-Efficiency-vs.-False-Positive-Rate