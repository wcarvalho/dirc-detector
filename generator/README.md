### Particle-Generator

<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

The main purpose of this library is to create the "**generator**" executable. Using the diagram above as reference, the executable will:
- emit up to N particles from point (0,0,0)
- determine if each particle will intersect with the DIRC at some coordinate (z<sub></sub>p, x<sub></sub>p, y<sub></sub>p)


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