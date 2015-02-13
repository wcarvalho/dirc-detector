lib_LTLIBRARIES = \
libDircObjs.la

include_HEADERS  =  \
	dirc_objects.h \
	particle_objects.h \
	photon_objects.h \
	analysis_objects.h \
	graphing.h


DircObjs_Dict.cpp: \
	dirc_objects.h \
	LinkDef.h
		rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^


libDircObjs_la_SOURCES = \
	analysis.cpp \
	dirc_objects.cpp \
	particle_objects.cpp \
	fitgaussianplusconstant.cpp \
	graphing.cpp \
	DircObjs_Dict.cpp

libDircObjs_la_LIBADD = \
$(ROOT_LIBS) \
$(UTIL_LIB) \
$(NEWTON_LIB)

libDircObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = DircObjs.pc


clean-local:
	rm -f $(BUILT_SOURCES) *Dict*