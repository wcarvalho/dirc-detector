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
- an executable which produces *N* events with up to *n* particles that would intersect with the modeled DIRC.
- etc.

### [Photon-Generator]
#####Used to generate Cherenkov light data from particles traversing through DIRC
######Contains:
- a class that simulates the trajectory of a particle within a rectangular prism 
- a class which rotates vectors
- etc.

### [Particle-Reconstructor]

### [Scripts]
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
