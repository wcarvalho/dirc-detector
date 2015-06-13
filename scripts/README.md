##Installation Instructions

Once you have cloned the master directory from git, you can




####(Calibration):[1]

####Photon Angle Smearing

####


```
g++ -std=c++11 -O3 -march=native -w -Wall Smearer.cpp -rpath `pkg-config --libs-only-L DircObjs | sed -e 's/\-L//g'``root-config --libs` -I`root-config --incdir` `pkg-config --libs DircObjs` `pkg-config --cflags DircObjs` -o ../smear
```


[1]:(https://github.com/wcarvalho/dirc-detector/tree/master/scripts#installation-instructions)