### Particle-Generator
####The main purpose of this library is to create the "**generator**" executable.
<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

Using the diagram above as reference, the executable works as follows:

1. <p> It emits particles from point (0,0,0) with a direction determined by solid angle (&eta;<sub>b</sub>, &theta;<sub>b</sub>)<p>
+ particle are emitted until either
    * The DIRC has the desired number of particles
    * Some upper threshold for the number of particles emitted has been met
3. <p> each particle that intersects with the DIRC has some coordinate (z<sub>p</sub>, x<sub>p</sub>, y<sub>p</sub>), solid angle (&theta;<sub>p</sub>, &phi;<sub>p</sub>) and &Beta; <p>
For more details on this process, please see: [nims-paper]

This executable creates N particles with properties supplied by a text file.

Sample file:
```
maxpars: 1
nparticles: 0,10000
eta: -.5, .5
pt: 0, 3
phi: .5, 1.5pi
charge: 0
types: electron, pion, kaon
replace
```




Generates particles for a number of events which can be specified by the user.
If you would like to generate the same particles every time, invoke option --random with a non-zero integer

##Installation Instructions

[nims-paper]:https://www.dropbox.com/s/ns3p81k5c5hysz2/nims_draft4.pdf?dl=0