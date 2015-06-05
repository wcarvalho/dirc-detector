You may contact with me concerns on this library through my gmail: wcarvalho92@gmail.com

This library was made for a project *modeling a Detection of Internally Reflected Cherenkov light ([DIRC]) particle detector* at Stony Brook University while I was there as an undergraduate student in physics. I worked on this project with professor Axel Drees and research professor [Alan Dion].
It has 4 main sub-libraries **Dirc-Objects**, **Particle-Generator**, **Photon-Generator**, and **Particle-Reconstructor**, and numerous **scripts** to streamline common tasks in this project.

### [Dirc-Objects]
#####Collection of ROOT classes to store relevant data
######Examples of data stored include:
- DIRC parameters
- particle data
- photon data
- particle reconstruction data

### [Particle-Generator]
#####Used to generate particles
######Contains:
- functions the generate particles
- an executable which produces *N* events with up to *n* particles that would intersect with the modeled DIRC.
![beams](https://github.com/wcarvalho/dirc-detector/blob/master/img/beams.jpg?raw=true)

### [Photon-Generator]
#####Used to generate Cherenkov light data from particles traversing through DIRC
######Contains:
- as
- as
![beams](https://github.com/wcarvalho/dirc-detector/blob/master/img/dirc.jpg?raw=true)
### [Particle-Reconstructor]

### [Scripts]
There are 3 pre-requisite libraries: 

1. [ROOT](https://root.cern.ch/drupal/)
2. [libseamstress](https://code.google.com/p/libseamstress/)
3. [fitnewton](https://code.google.com/p/fitnewton/)


**Note:** Installation for this library has only been tested on Mac OSX 10.0+ and Linux Ubuntu 14.0+. It has `NOT` been tested for Windows. While this installation may work on other UNIX-based systems (and windows builds), this is not guarenteed and you continue with installation at your own risk.

[Dirc-Objects]:https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects
[Particle-Generator]:https://github.com/wcarvalho/dirc-detector#particle-generator
[Photon-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/generator
[Particle-Reconstructor]:https://github.com/wcarvalho/dirc-detector/tree/master/simulator
[scripts]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts
[DIRC]:http://en.wikipedia.org/wiki/Detection_of_internally_reflected_Cherenkov_light
[Alan Dion]: https://github.com/alandion
