AC_INIT(DircObjs, 0.0.1, wilka.carvalho@stonybrook.edu)

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(dirc_objects.h)

AM_INIT_AUTOMAKE([foreign])

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -std=c++11 -pipe -Wno-deprecated-register -O3 -march=native -I`root-config --incdir` `pkg-config --cflags fitnewton`"

ROOT_LIBS=`root-config --libs`
NEWTON_LIB=`pkg-config --libs fitnewton`

AC_SUBST(ROOT_LIBS)
AC_SUBST(UTIL_LIB)
AC_SUBST(NEWTON_LIB)

PKG_CHECK_MODULES([Fitnewton], [fitnewton])

AC_CONFIG_FILES([Makefile DircObjs.pc])

AC_OUTPUT
