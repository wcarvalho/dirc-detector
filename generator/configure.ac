AC_INIT([generator], [1], [wilka.carvalho@stonybrook.edu] )

AC_CONFIG_AUX_DIR(config)

AM_INIT_AUTOMAKE

AC_PROG_CXX
AC_PROG_LIBTOOL

##CXXFLAGS="-Wall -Werror -O2 ${CXXFLAGS}"
CPPFLAGS="${CPPFLAGS} -I`root-config --incdir` `pkg-config --cflags DircObjs`"

ROOT_LIBS=`root-config --libs`
LIB=`pkg-config --libs DircObjs`

AC_SUBST(ROOT_LIBS)
AC_SUBST(LIB)

AC_CONFIG_FILES([Makefile])
AC_OUTPUT