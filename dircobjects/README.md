

##Installation Instructions

Once you have cloned the master directory from git, you can begin the process of building this library via
```
cd dircobjects
autoreconf --force --install
```
To facilitate linking to this library, I recommend installing this library in one of your pkg-config paths (i.e. setting your prefix to your pkg-config directory). One can do so as follows
```
./configure --prefix=$PKG_CONFIG_PATH
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