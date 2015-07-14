AC_INIT([reconstructor], [1], [wilka.carvalho@stonybrook.edu] )

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -std=c++11 -pipe -Wno-deprecated-register -O3 -march=native -I`root-config --incdir` `pkg-config --cflags DircObjs` `pkg-config --cflags SimObjs` `pkg-config --cflags fitnewton`"

ROOT_LIBS=`root-config --libs -lTSpectrum`
DIRC_LIB=`pkg-config --libs DircObjs`
SIM_LIB=`pkg-config --libs SimObjs`
NEWTON_LIB=`pkg-config --libs fitnewton`

AC_SUBST(ROOT_LIBS)
AC_SUBST(DIRC_LIB)
AC_SUBST(SIM_LIB)
AC_SUBST(UTIL_LIB)
AC_SUBST(NEWTON_LIB)


PKG_CHECK_MODULES([Fitnewton], [fitnewton])

AC_CONFIG_FILES([Makefile ReconstructionObjs.pc])
AC_OUTPUT
