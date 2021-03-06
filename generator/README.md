# Particle-Generator
###The main purpose of this library is to create the "**generator**" executable. Here you can find:
####1. a summary of the executable's [command-line options](https://github.com/wcarvalho/dirc-detector/tree/master/generator#summary-of-command-line-options) ([CLICK HERE](https://github.com/wcarvalho/dirc-detector/tree/master/generator#details-on-command-line-options) for details on the options)
####2. details on the library's [functions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#functions)
####3. the library's [installation instructions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)

---
##Figure of Particle Generation
<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

###Definitions:
<p> N: number of particles emitted from (0,0,0) <p>
<p> N<sup>&darr;</sup>: minimum number of particles emitted from (0,0,0) <p>
<p> N<sup>&uarr;</sup>: maximum number of particles emitted from (0,0,0) <p>
<p> N<sub>DIRC</sub>: number of particles that intersect with the DIRC <p>
<p> N<sup>&uarr;</sup><sub>DIRC</sub>: maximum number of particles to intersect with the DIRC <p>

###Using the diagram above as reference, the executable works as follows:

1. <p> It emits particles from point (0,0,0) with a direction determined by solid angle (&eta;<sub>b</sub>, &phi;<sub>b</sub>)<p>
+ Particles are emitted until either
    * <p> N<sub>DIRC</sub> = N<sup>&uarr;</sup><sub>DIRC</sub> or <p>
    * <p> N = N<sup>&uarr; <p>
+ <p> Each particle that intersects with the DIRC has some coordinate (z<sub>p</sub>, x<sub>p</sub>, y<sub>p</sub>), solid angle (&theta;<sub>p</sub>, &phi;<sub>p</sub>) and &Beta;<p>
**For more details on this process, please see the [related paper][1]**

##Summary of command line options:

| Command | Description|
| :--- | :--- |
| [-f / --filename][7] | sets the output-file name  |
| [-e / --events][3] | sets the number of events |
| [-m / --maxpars][5] | sets the maximum number of particles to intersect with the DIRC per event (see [Figure of Particle Generation][2] |
| [-P / --particle][4] | sets the number of particles that are emitted from (0,0,0) (see [Figure of Particle Generation][2]|
| [-r / --random][6] | sets the seed for the random number generator |
| [-D / --Directory][9] | sets the directory of the output-file |
| [--pt-distribution-function][10] | sets a custom function for the pt probability-distribution of the particles |
| [-c / --custom-set][8] | customizes the particle generation |

##Details on command line options:

####f/filename
  + takes in **1 string** argument to set the output-file name (output file is a root file)
  + e.g. to create a file named "example_file_name.root", do
```
generator -f example_file_name.root
```

####e/events
  + takes in **1 int** argument for the number of events
  + e.g. to create a file with 10 events, do
```
generator -e 10
```
####m/maxpars
  + <p> takes in **1 int** argument for N<sup>&uarr;</sup><sub>DIRC</sub> <p>
  + e.g. to have at most 10 particles intersect the DIRC, do
```
generator -m 10
```

####P/particle
  + <p> takes in **2 int** arguments for N<sub>&darr;</sub> and N<sub>&uarr;</sub><p>
  + e.g. to generate at least 100 particles but at most 500, do
```
generator -P 100 500
```
  + <p> if `maxpars` is set, particles are emitted until N >= N<sup>&darr;</sup> and N<sub>DIRC</sub> = N<sup>&uarr;</sup><sub>DIRC</sub> <p>
    + <p> if N<sub>DIRC</sub> = N<sup>&uarr;</sup><sub>DIRC</sub> is never true, particles are emitted until N = N<sup>&uarr;</sup> <p>
  + e.g. if one wants at most 1000 particles emitted but at most 4 to intersect with the DIRC, one would do
```
generator -P 0 1000 -m 4
```


####r/random
  + takes in **1 int** argument to set the seed for the random number generator
  + if r is set to 0, all numbers are random
  + e.g. files output1.root and output2.root will have different data
    + further, each file will have different data on each file generation
```
generator -f output1.root -r 0
generator -f output2.root -r 0
```
  + if r is not set to 0, all random numbers chosen will repeat in each run of the executable
  + e.g. files output1.root and output2.root will have the same data
    + further, each file will have the same data on each file generation
```
generator -f output1.root -r 1
generator -f output2.root -r 1
```
  + the data generated corresponds to the seed used, so different seeds have different data.
  + e.g. files output1.root and output2.root will have different data
```
generator -f output1.root -r 1
generator -f output2.root -r 2
```
####D/Directory
  + takes in **1 string** argument to set the directory of the output-file
  + e.g. to save the file to `/some/path/in/your/computer`, do
```
generator -D /some/path/in/your/computer
```
####pt-distribution-function
  + takes in **1 string** argument to set the probability-distribution for the pt values (in GeV/c) of the emitted particles
  + one can use the custom function `x/(0.5+x*x*x*x)` as follows
```
generator --pt-distribution-function x/(0.5+x*x*x*x)
```
  + piece-wise functions are supported
  + e.g. to have a uniform 70% chance that particles will have pt below 2Gev/c and a uniform 30% chance that particles will have pt equal to or above 2Gev/c, do
```
generator --pt-distribution-function (x<2)*.7+(x>=2)*.3
```

####c/custom-set
  + this takes in **1 string** argument that corresponds to a textfile that will set the details for the particle generation.
  + this command can set the variables `maxpars`, `particle` along with other paramaters.
  + it can set
    + maxpars, the maximum number of particles to intersect the DIRC
    + nparticles, the minimum and maximum number of particles to be emitted
    + eta, the eta range for the particles
    + pt, the pt range for the particles
    + phi, the phi range for the particles
      + this argument recognizes `pi`, e.g. 2pi is recognized
    + charge, the charge distribution of the particles
      + if *set to 1*, all particles will have a positive charge
      + if *set to -1*, all particles will have a negative charge
      + if *set to 0*, all particles will a random charge
    + types, the types of particles that are generated
      + options: electrons, pions, kaons, muons, protons
    + replace, a boolean for whether particles with the default parameters are generated in addition to this set or noot
      + if this *is not* set, both sets of particles are generated
      + if this *is* set, only this custom set is generated

  + to use the file `custom-details.txt`, one would do
```
generator -c "custom-details.txt"
```
  + example parameters could be
```
// custom-details.txt
maxpars: 1                     // The desired number of particles in the DIRC is 1
nparticles: 0,10000            // Up to 1000 particles will be emitted from (0,0,0)
eta: -.5, .5                   // eta range
pt: 0, 3                       // pt range
phi: .5, 1.5pi                 // phi range
charge: 0                      // randomly charged particles
types: electron, pion, kaon    // only electrons, pions, and kaons will be emitted
replace                        // with this option enabled, this set supplants the default set that would be created
```
  + The generator, by default, creates particles with the following parameters:
```
maxpars: 5
nparticles: 0,10000
eta: -.5, .5
pt: 0, 3
phi: 0, 2pi
charge: 0
types: electron, pion, kaon, muon, proton
replace
```

##Functions

##Installation Instructions

Once you have cloned the master directory from git, you can begin the process of building this library via
```
cd generator
autoreconf --force --install
```
You can configure with `./configure`. You can set the prefix as you normally would for a library installed with pkg-config. This simply requires that you set your prefix to a prefix recognized by pkg-config. In order to have this library's executable accessible throughout your system, you must set bindir to a location in your path. One can do both as follows
```
./configure --prefix=$PKG_CONFIG_PATH --bindir=$PATH
```
You can then build and install via
```
make
make install
```
To force a fresh release after an earlier checkout do
```
make distclean
git pull
./configure
make
make install
```

[1]:http://1drv.ms/1NPyAHS
[2]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#figure-of-particle-generation
[3]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#eevents
[4]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#pparticle
[5]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#mmaxpars
[6]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#rrandom
[7]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ffilename
[8]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ccustom-set
[9]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ddirectory
[10]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#pt-distribution-function
