# Particle-Generator
###The main purpose of this library is to create the "**generator**" executable. Here you can find:
####1. a summary of the executable's [command-line options](https://github.com/wcarvalho/dirc-detector/tree/master/generator#summary-of-command-line-options) ([CLICK HERE](https://github.com/wcarvalho/dirc-detector/tree/master/generator#command-line-options) for details on the options)
####2. details on the library's [functions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#functions)
####3. the library's [installation instructions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)

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
| -e / --events | sets the number of events |
| -P / --particle | sets the number of particles that are emitted from (0,0,0) (see [Figure of Particle Generation](https://github.com/wcarvalho/dirc-detector/tree/master/generator#Figure-of-Particle-Generation)|
| -m / --maxpars | sets the maximum number of particles to intersect with the DIRC per event |
| -r / --random | sets the seed for the random number generator |
| -f / --filename | sets the output-file name  |
| -c / --custom-set | customizes the particle generation |
| -D / --Directory | sets the directory of the output-file |
| --pt-distribution-function | sets a custom function for the pt probability-distribution of the particles |


##Command line options:
#####e/events
  + takes in one int argument for the number of events
  + e.g. `generator -e 10` creates a file with 10 events
#####P/particle
  + takes in 2 int arguments for the minimum and maximum number of particles emitted from the
  + e.g. `generator -e 10` creates a file with 10 events
#####m/maxpars
`generator `
#####r/random
    + if r=0 (i.e. `generator -r 0`), all numbers are random
    + if r /= 0 (e.g. `generator -r 3`), all random numbers chosen will repeat in each run of the executable
#####f/filename
    + sets the output filename (output file is a root file)
    + e.g. `generator -f example_file_name.root` will output example_file_name.root
#####c/custom-set
`generator -c "custom-details.txt"`
#####D/Directory
`generator `
#####pt-distribution-function
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