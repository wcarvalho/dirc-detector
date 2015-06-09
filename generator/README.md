### Particle-Generator
####The main purpose of this library is to create the "**generator**" executable.
<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

Using the diagram above as reference, the executable works as follows:

1. <p> It emits particles from point (0,0,0) with a direction determined by solid angle (&eta;<sub>b</sub>, &phi;<sub>b</sub>)<p>
+ particle are emitted until either
    * The DIRC has the desired number of particles
    * Some upper threshold for the number of particles emitted has been met
+ <p> each particle that intersects with the DIRC has some coordinate (z<sub>p</sub>, x<sub>p</sub>, y<sub>p</sub>), solid angle (&theta;<sub>p</sub>, &phi;<sub>p</sub>) and &Beta;<p>

**For more details on this process, please see the [related paper][1]**

Details on this generation process are provided via text file. A sample file:

####Command line options:


+ **One can provide the details of the generation process in a text file via the  f/custom-set command**:
	+ `generator -c $parameters`
	+ below is a sample text file, parameters.txt
```
// parameters.txt
maxpars: 1													// The desired number of particles in the DIRC is 1
nparticles: 0,10000									// Up to 1000 particles will be emitted from (0,0,0)
eta: -.5, .5												// eta range
pt: 0, 3														// pt range
phi: .5, 1.5pi 											// phi range
charge: 0														// charge of the particles
types: electron, pion, kaon					// types of particles that will be emitted
replace															// if
```

+ **# of events**:
	`generator -e 10 -f $particles_file -c $parameters`

Generates particles for a number of events which can be specified by the user.
If you would like to generate the same particles every time, invoke option --random with a non-zero integer

##Installation Instructions

[1]:https://www.dropbox.com/s/ns3p81k5c5hysz2/nims_draft4.pdf?dl=0