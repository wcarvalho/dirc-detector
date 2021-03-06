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


###pre-requisite libraries:

1. [ROOT](https://root.cern.ch/drupal/) (version 5.34)
+ [eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)
+ [libseamstress](https://code.google.com/p/libseamstress/) (please use revision 21, i.e. do `svn update -r21`)
+ [fitnewton](https://code.google.com/p/fitnewton/)

Note: For libseamstress and fitnewton, please make sure that the `configure.ac` files are using C++11

####Easy installation
Dirc-Detector facilitate the install process by including shell scripts in the build_scripts directory that can be used to install all (or any) of its libraries and scripts. In order to do so, you will need to run the shell scripts with the appropriate environment variables.

**Environment variables to export**

1. *MYLIBS* : used as location to install libraries
+ *MYAPPS* : used as location to install executables
+ *scripts*  : used as location to install executable scripts

Note: Please make sure **MYAPPS** and **scripts** are in your `$PATH`

######Installing libraries
The easiest thing to do is to create a link to all shell scripts in `build_scripts` in your main directory, i.e.
```
ln build_scripts/*.sh .
```
Although, you can link only the shell script files described below, i.e.

```
ln build_scripts/*dircobjects*.sh .
ln build_scripts/*simulator*.sh .
ln build_scripts/*generator*.sh .
ln build_scripts/*reconstructor*.sh .
```

For each library do:

1. `bash autoreconf_library.sh`
+ `bash configure_library.sh`
+ `bash install_library.sh`

For example:
```
bash autoreconf_dircobjects.sh
bash configure_dircobjects.sh
bash install_dircobjects.sh
```

Recommended order: (1) dircobjects, (2) simulator, (3) generator, (4) reconstructor

<!-- ######Installing scripts


All shell scripts to built C++ scripts have prefix "install_" (e.g. install_generator.sh, install_dircobjects.sh, etc.)
 -->

####Harder installation
If the installation instructions above didn't work out for some reason, you can follow the installation instruction in each component's README.

1. [Dirc-Objects](https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects#installation-instructions)
2. [Particle-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/generator#installation-instructions)
3. [Photon-Generator](https://github.com/wcarvalho/dirc-detector/tree/master/simulator#installation-instructions)
4. [Particle-Reconstructor](https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor#installation-instructions)
5. [Scripts](https://github.com/wcarvalho/dirc-detector/tree/master/scripts#installation-instructions)

You may contact me with any concerns about this library at wcarvalho92@gmail.com

**Note:** Installation for this library has only been tested on [Mac OSX 10.0+](https://www.apple.com/osx/) and [Linux Ubuntu 14.0+](http://www.ubuntu.com/download/desktop). It has <span style="color:red;font-weight:bold">NOT</span> been tested for Windows. While this installation may work on other UNIX-based systems (and windows builds), this is not guaranteed and you continue with installation at your own risk.


[Dirc-Objects]:https://github.com/wcarvalho/dirc-detector/tree/master/dircobjects
[Particle-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/generator
[Photon-Generator]:https://github.com/wcarvalho/dirc-detector/tree/master/simulator
[Particle-Reconstructor]:https://github.com/wcarvalho/dirc-detector/tree/master/reconstructor
[scripts]:https://github.com/wcarvalho/dirc-detector/tree/master/scripts
