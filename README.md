<script src='https://cdn.mathjax.org/mathjax/latest/MathJax.js?config=TeX-AMS-MML_HTMLorMML'></script>

#Modeling a DIRC for high-multiplicity collisions

This library was made for a project *modeling a Detection of Internally Reflected Cherenkov light ([DIRC](http://en.wikipedia.org/wiki/Detection_of_internally_reflected_Cherenkov_light)) particle detector* at Stony Brook University. I collaborated with professor Axel Drees and research professor [Alan Dion](https://github.com/alandion).
It has 4 main sub-libraries **Dirc-Objects**, **Particle-Generator**, **Photon-Generator**, and **Particle-Reconstructor**, and numerous **scripts** to streamline common tasks in this project.

### [Dirc-Objects]
#####Collection of ROOT classes to store relevant data
######Examples of data stored include:
- DIRC parameters
- particle data
- photon data
- particle reconstruction data
- etc.

### [Particle-Generator]
#####Used to generate particles
######Contains:
- functions the generate particles
- an executable that produces *N* events with up to *n* particles that would intersect with the modeled DIRC.

### [Photon-Generator]
#####Used to generate Cherenkov light data from particles traversing through DIRC
######Contains:
- a class that simulates the trajectory of a particle within a rectangular prism 
- a class which rotates vectors
- an executable that simulates the trajectory of particles in the modeled DIRC and produces the Cherenkov light data that would be read from the DIRC surface
- etc.

### [Particle-Reconstructor]
#####Used to reconstruct the original particles
######Contains:
- a function that fits $$$f(\theta) = a \cdot exp\left(\frac{(\theta - b)^2}{c^2})\right+d$$$ to a ROOT histogram
- a function that calculates the number of photons expected to be detected by the DIRC for a particular particle
- an executable that determines the likelihood that each particle is a certain particle type
- etc.

### [Scripts]
######examples of script functions include:
- embedding a particle into a particle set
- smearing the angles of the photons
- etc.


---
There are 3 pre-requisite libraries: 

1. [ROOT](https://root.cern.ch/drupal/)
2. [libseamstress](https://code.google.com/p/libseamstress/)
3. [fitnewton](https://code.google.com/p/fitnewton/)

You may contact with me any concerns with this library via: wcarvalho92@gmail.com

**Note:** Installation for this library has only been tested on Mac OSX 10.0+ and Linux Ubuntu 14.0+. It has `NOT` been tested for Windows. While this installation may work on other UNIX-based systems (and windows builds), this is not guarenteed and you continue with installation at your own risk.


[Dirc-Objects]:https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects
[Particle-Generator]:https://github.com/wcarvalho/dirc-detector#particle-generator
[Photon-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/generator
[Particle-Reconstructor]:https://github.com/wcarvalho/dirc-detector/tree/master/simulator
[scripts]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts
