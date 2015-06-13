### Photon-Generator

<figure>
	<img src="https://github.com/wcarvalho/dirc-detector/blob/master/img/dirc.jpg?raw=true" alt="dirc" style="width: 400px;"/>
  <figcaption> </figcaption>
</figure>

##The main purpose of this library is to create the "**simulator**" executable. TH


##Installation Instructions

Once you have cloned the master directory from git, you can begin the process of building this library via
```
cd generator
autoreconf --force --install
```
To facilitate linking to this library, I recommend installing this library in one of your pkg-config paths (i.e. setting your prefix to your pkg-config directory). This library also contains an executable. In order to use this executable, you must set bindir to a location in your path. One can do both as follows
```
./configure --prefix=$PKG_CONFIG_PATH --bindir=$PATH
```
You can then build via
```make
```
To force a fresh release after an earlier checkout do
```
make distclean
git pull
./configure
make
```
You can install with
```
make install
```