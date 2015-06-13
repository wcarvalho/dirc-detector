# Particle-Generator
###The main purpose of this library is to create the "**generator**" executable. Here you can find:
####1. a summary of the executable's [command-line options](https://github.com/wcarvalho/dirc-detector/tree/master/generator#summary-of-command-line-options) ([CLICK HERE](https://github.com/wcarvalho/dirc-detector/tree/master/generator#command-line-options-use) for details on the options)
####2. details on the library's [functions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#functions)
####3. the library's [installation instructions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)

---
##Figure of Particle Generation
<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

Using the diagram above as reference, the executable works as follows:

1. <p> It emits particles from point (0,0,0) with a direction determined by solid angle (&eta;<sub>b</sub>, &phi;<sub>b</sub>)<p>
+ Particles are emitted until either
    * The DIRC has the desired number of particles
    * Some upper threshold for the number of particles emitted has been met
+ <p> Each particle that intersects with the DIRC has some coordinate (z<sub>p</sub>, x<sub>p</sub>, y<sub>p</sub>), solid angle (&theta;<sub>p</sub>, &phi;<sub>p</sub>) and &Beta;<p>
**For more details on this process, please see the [related paper][1]**

##Summary of command line options:

| Command | Description|
| :--- | :--- |
| [-e / --events][3] | sets the number of events |
| [-P / --particle][4] | sets the number of particles that are emitted from (0,0,0) (see [Figure of Particle Generation][2]|
| [-m / --maxpars][5] | sets the maximum number of particles to intersect with the DIRC per event (see [Figure of Particle Generation][2] |
| [-r / --random][6] | sets the seed for the random number generator |
| [-f / --filename][7] | sets the output-file name  |
| [-c / --custom-set][8] | customizes the particle generation |
| [-D / --Directory][9] | sets the directory of the output-file |
| [--pt-distribution-function][10] | sets a custom function for the pt probability-distribution of the particles |

##Command line options use:

###Definitions:
<p> N: number of particles emitted from (0,0,0) <p>
<p> N<sup>&darr;</sup>: minimum number of particles emitted from (0,0,0) <p>
<p> N<sup>&uarr;</sup>: maximum number of particles emitted from (0,0,0) <p>
<p> N<sub>DIRC</sub>: number of particles that intersect with the DIRC <p>
<p> N<sup>i</sup><sub>DIRC</sub>: maximum number of particles to intersect with the DIRC <p>

####f/filename
  + sets the output-file name (output file is a root file)
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

####P/particle
  + <p> takes in **2 int** arguments for N<sub>&darr;</sub> and N<sub>&uarr;</sub><p>
  + e.g. to generate at least 100 particles but at most 500, do
```
generator -P 100 500
```
  + <p> if `maxpars` is set, particles are emitted until N >= N<sup>&darr;</sup> and N<sub>DIRC</sub> = N<sup>i</sup><sub>DIRC</sub> <p>
    + <p> if N<sub>DIRC</sub> = N<sup>i</sup><sub>DIRC</sub> is never true, particles are emitted until N = N<sup>&uarr;</sup> <p>
  + e.g. if one wants at most 1000 particles emitted but at most 4 to intersect with the DIRC, one would do
```
generator -P 0 1000 -m 4
```

####m/maxpars
  + <p> takes in **1 int** argument for N<sup>i</sup><sub>DIRC</sub> <p>
  + e.g. to have at most 10 particles intersect the DIRC, do
```
generator -m 10
```

####r/random
  + sets the seed for the random number generator
  + if r is set to 0, all numbers are random
  + e.g. files output1.root and output2.root will have different data
```
generator -f output1.root -r 0
generator -f output2.root -r 0
```
  + if r is not set to 0, all random numbers chosen will repeat in each run of the executable
  + e.g. files output1.root and output2.root will have the same data
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

##Unfinished

####c/custom-set
`generator -c "custom-details.txt"`

####D/Directory
`generator `

####pt-distribution-function
`generator `








user-chosen options via commandline options

The generator, by default, creates particles with the following parameters:
```
// custom-details.txt
maxpars: 5                     // The desired number of particles in the DIRC is 1
nparticles: 0,10000            // Up to 1000 particles will be emitted from (0,0,0)
eta: -.5, .5                   // eta range
pt: 0, 3                       // pt range
phi: 0, 2pi                 // phi range
charge: 0                                    // charge of the particles
types: electron, pion, kaon, muon, proton    // types of particles that will be emitted
replace                        // only generates custom set
```
+ One can create a custom set of particles, either in addition to or in place of the default set via the following
	+ **command:** c/custom-set
	<!-- + **function:** Used to provide the details for a custom generation process via a text file -->
	+ **example:** `generator -c "custom-details.txt"`
	+ **sample:**
```
// custom-details.txt
maxpars: 1                     // The desired number of particles in the DIRC is 1
nparticles: 0,10000            // Up to 1000 particles will be emitted from (0,0,0)
eta: -.5, .5                   // eta range
pt: 0, 3                       // pt range
phi: .5, 1.5pi                 // phi range
charge: 0                      // charge of the particles
types: electron, pion, kaon    // types of particles that will be emitted
replace                        // with this option enabled, this set supplants the original set
```


+ **One can provide the details for a custom generation process in a text file via the c/custom-set command**:
	+ `generator -c "custom-details.txt"`
	+
	+ below is an example text file

+ **# of events**:
	`generator -e 10 -f $particles_file -c $parameters`

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

[1]:https://www.dropbox.com/s/ns3p81k5c5hysz2/nims_draft4.pdf?dl=0
[2]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#Figure-of-Particle-Generation
[3]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#eevents
[4]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#pparticle
[5]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#mmaxpars
[6]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#rrandom
[7]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ffilename
[8]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ccustom-set
[9]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#ddirectory
[10]:https://github.com/wcarvalho/dirc-detector/tree/master/generator#pt-distribution-function
