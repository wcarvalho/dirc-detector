#Modeling a DIRC for high-multiplicity collisions

This is a C++11 library made for a project *modeling a Detection of Internally Reflected Cherenkov light ([DIRC](http://en.wikipedia.org/wiki/Detection_of_internally_reflected_Cherenkov_light)) particle detector* at Stony Brook University. I collaborated with professor Axel Drees and research professor [Alan Dion](https://github.com/alandion).
This library has 5 components: 4 sub-libraries **(1) Dirc-Objects**, **(2) Particle-Generator**, **(3) Photon-Generator**, **(4) Particle-Reconstructor**, and a set of **(5) scripts** to streamline common tasks.

---

**Click [HERE](https://github.com/wcarvalho/dirc-detector#installation-instructions) to skip to the installation instructions**

---
## Brief Description of Components
### 1. [Dirc-Objects]
#####Collection of ROOT classes to store data
######Examples of data stored include:
- DIRC parameters
- particle data
- photon data
- particle reconstruction data

### 2. [Particle-Generator]
#####Used to generate particles
######Main Function:
- an executable that produces *N* events with up to *n* particles that would intersect with the modeled DIRC.

### 3. [Photon-Generator]
#####Used to generate Cherenkov light data from particles traversing through DIRC
######Main Function:
- an executable that simulates the trajectory of particles in the modeled DIRC and produces the Cherenkov light data that would be read from the DIRC photon detector

### 4. [Particle-Reconstructor]
#####Used to reconstruct original particles from photon data
######Contains:
- a function that calculates the number of photons expected to be detected by the DIRC for a particular particle
- a function that indexes photons by the particle they were most likely emitted from
- an executable that determines the likelihood that each particle is a certain particle type
- etc.

### 5. [Scripts]
######Examples of script functions include:
- embedding a particle into a particle set
- smearing the angles of a set of photons

---
##Installation Instructions
There are numerous pre-requisite libraries: 

1. [ROOT](https://root.cern.ch/drupal/) (version 5.34)
2. [pkg-config](http://www.freedesktop.org/wiki/Software/pkg-config/)
3. [svn](https://subversion.apache.org/)
2. [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
2. [libseamstress](https://code.google.com/p/libseamstress/)
3. [fitnewton](https://code.google.com/p/fitnewton/)

Instructions on how to install each component of this library are located within each component's README:

1. [Dirc-Objects](https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects#installation-instructions)
2. [Particle-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)
3. [Photon-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/simulator#installation-instructions)
4. [Particle-Reconstructor](https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor#installation-instructions)
5. [Scripts](https://github.com/wcarvalho/dirc-detector/tree/master/scripts#installation-instructions)

You may contact with me any concerns about this library via wcarvalho92@gmail.com

**Note:** Installation for this library has only been tested on [Mac OSX 10.0+](https://www.apple.com/osx/) and [Linux Ubuntu 14.0+](http://www.ubuntu.com/download/desktop). It has <span style="color:red;font-weight:bold">NOT</span> been tested for Windows. While this installation may work on other UNIX-based systems (and windows builds), this is not guarenteed and you continue with installation at your own risk.


[Dirc-Objects]:https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects
[Particle-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/generator
[Photon-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/simulator
[Particle-Reconstructor]:https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor
[scripts]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts
