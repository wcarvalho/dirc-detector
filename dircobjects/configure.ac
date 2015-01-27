AC_INIT(DircObjs, 0.0.1, wilka.carvalho@stonybrook.edu)

AC_CONFIG_AUX_DIR(config)
AC_CONFIG_SRCDIR(dirc_objects.h)

AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -I`root-config --incdir`"

AC_SUBST(ROOT_LIBS)
ROOT_LIBS=`root-config --libs`

AC_CONFIG_FILES([Makefile DircObjs.pc])

AC_OUTPUT
