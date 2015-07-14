#Modeling a DIRC for high-multiplicity collisions

This is a C++11 library made for a project *modeling a Detection of Internally Reflected Cherenkov light ([DIRC](http://en.wikipedia.org/wiki/Detection_of_internally_reflected_Cherenkov_light)) particle detector* at Stony Brook University. I collaborated with professor Axel Drees and research professor [Alan Dion](https://github.com/alandion).
This library has 5 components: 4 sub-libraries **(1) Dirc-Objects**, **(2) Particle-Generator**, **(3) Photon-Generator**, **(4) Particle-Reconstructor**, and a set of **(5) scripts** to streamline common tasks.

---

**To skip to the installation instructions** [CLICK HERE](https://github.com/wcarvalho/dirc-detector#installation-instructions)

---
## Brief Description of Components
### 1. [Dirc-Objects]
#####Contains a collection of ROOT classes to store data. Examples of data stored include:
- DIRC parameters
- particle data
- photon data
- particle reconstruction data

### 2. [Particle-Generator]
#####Contains an executable and functions to generate particles and intersect them with the modeled DIRC.

### 3. [Photon-Generator]
#####Contains an executable and functions to simulate the trajectory of particles within the modeled DIRC and produce the Cherenkov light data that would be read from the DIRC photon detector.

### 4. [Particle-Reconstructor]
#####Contains an executable and functions to disentangle the Cherenkov light data and reconstruct the identity of the particles that produced this light.

### 5. [Scripts]
#####Contains scripts for common tasks in this project such as embedding particle data into an existing data set or smearing the angles of a set of photons.

---
##Installation Instructions
Before continuing, make sure that *libtool, pkg-config, autoconf,* and *automake* are installed.


There are numerous pre-requisite libraries:

1. [ROOT](https://root.cern.ch/drupal/) (version 5.34)
+ [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
Please make sure that the `configure.ac` files are using C++11
+ [libseamstress](https://code.google.com/p/libseamstress/) (please use revision 21, i.e. do `svn update -r21`)
+ [fitnewton](https://code.google.com/p/fitnewton/)

Instructions on how to build and install each component of this library are located within each component's README:

1. [Dirc-Objects](https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects#installation-instructions)
2. [Particle-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)
3. [Photon-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/simulator#installation-instructions)
4. [Particle-Reconstructor](https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor#installation-instructions)
5. [Scripts](https://github.com/wcarvalho/dirc-detector/tree/master/scripts#installation-instructions)

You may contact me with any concerns about this library via wcarvalho92@gmail.com

**Note:** Installation for this library has only been tested on [Mac OSX 10.0+](https://www.apple.com/osx/) and [Linux Ubuntu 14.0+](http://www.ubuntu.com/download/desktop). It has <span style="color:red;font-weight:bold">NOT</span> been tested for Windows. While this installation may work on other UNIX-based systems (and windows builds), this is not guaranteed and you continue with installation at your own risk.


[Dirc-Objects]:https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects
[Particle-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/generator
[Photon-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/simulator
[Particle-Reconstructor]:https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor
[scripts]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts
