### Photon-Generator

<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/dirc.jpg?raw=true" alt="dirc" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

##The main purpose of this library is to create the "**simulator**" executable. TH


##Installation Instructions

Once you have cloned the master directory from git, you can begin the process of building this library via
```
cd simulator
autoreconf --force --install
```
You can configure with `./configure`. You can set the prefix as you normally would for a library installed with pkg-config. This simply requires that you set your prefix to a prefix recognized by pkg-config. In order to have this library's executable accessible throughout your system, you must set bindir to a location in your path. One can do both as follows
```
./configure --prefix=$PKG_CONFIG_PATH --bindir=$PATH
```
You can then build and install via
```
make
make install
```
To force a fresh release after an earlier checkout do
```
make distclean
git pull
./configure
make
make install
```