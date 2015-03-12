AC_INIT([simulator], [1], [wilka.carvalho@stonybrook.edu] )

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -std=c++11 -pipe -Wno-deprecated-register -O3 -march=native -I`root-config --incdir` `pkg-config --cflags DircObjs`"

ROOT_LIBS=`root-config --libs`
DIRC_LIB=`pkg-config --libs DircObjs`

AC_SUBST(ROOT_LIBS)
AC_SUBST(DIRC_LIB)

AC_CONFIG_FILES([Makefile SimObjs.pc])
AC_OUTPUT
