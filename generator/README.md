### Particle-Generator

<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true" alt="beams" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

The main purpose of this library is to create the "**generator**" executable. This executable creates N particles with properties supplied by a text file.

Sample file:
```
maxpars: 1
nparticles: 0,10000
eta: -.5, .5
pt: .2, .3
phi: .75pi, 1.5pi
charge: 0
types: electron
replace
```




Generates particles for a number of events which can be specified by the user.
If you would like to generate the same particles every time, invoke option --random with a non-zero integer

##Installation Instructions