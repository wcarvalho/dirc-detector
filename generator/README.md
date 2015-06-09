# Particle-Generator
###The main purpose of this library is to create the "**generator**" executable. Here you can find:
####1. the executable's [Command-line options](https://github.com/wcarvalho/dirc-detector/tree/master/generator#command-line-options)
####2. the library's [Installation instructions](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)

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

##Command line options:
####+. e/events: # of events
####+. P/particle: # of particles thrown
####+. m/maxpars: The maximum # of particles that will intersect with the DIRC
####+. r/random: Seed value

   +. if r=0, all numbers are random
   +. if r /= 0, numbers chosen by the random number generator will repeat in each run of the executable

####+. f/filename: output filename
####+. c/custom-set
####+. D/Direction: directory in which files will be saved
####+. pt-distribution-function: function that determines the probability distribution for different pt values

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

##Installation Instructions

[1]:https://www.dropbox.com/s/ns3p81k5c5hysz2/nims_draft4.pdf?dl=0