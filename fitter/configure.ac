AC_INIT([fitter], [1], [wilka.carvalho@stonybrook.edu] )

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -std=c++11 -pipe -Wno-deprecated-register -O3 -march=native -I`root-config --incdir` `pkg-config --cflags DircObjs` `pkg-config --cflags fitnewton`"

ROOT_LIBS=`root-config --libs`
DIRC_LIB=`pkg-config --libs DircObjs`
NEWTON_LIB=`pkg-config --libs fitnewton`
SEAMSTRESS_LIB=`pkg-config --libs seamstress`

AC_SUBST(ROOT_LIBS)
AC_SUBST(DIRC_LIB)
AC_SUBST(NEWTON_LIB)
AC_SUBST(SEAMSTRESS_LIB)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT