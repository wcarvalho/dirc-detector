

##Installation Instructions

Once you have cloned the master directory from git, you can begin the process of building this library via
```
cd dircobjects
autoreconf --force --install
```
You can configure with `./configure`. You can set the prefix as you normally would for a library installed with pkg-config. This simply requires that you set your prefix to a prefix recognized by pkg-config. One can do so as follows
```
./configure --prefix=$PKG_CONFIG_PATH
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