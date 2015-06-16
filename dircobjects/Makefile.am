lib_LTLIBRARIES = \
libDircObjs.la

include_HEADERS  =  \
	dirc_objects.h \
	particle_objects.h \
	photon_objects.h \
	analysis_objects.h \
	event_parsers.h \
	dircTH1D.h \
	utility_library.h \
	FileProperties.h \
	calibration_data.h

# dircComparisons.h
# dircTFile.h
# dirc_io.h


DircObjs_Dict.cpp: \
	dirc_objects.h \
	particle_objects.h \
	photon_objects.h \
	analysis_objects.h \
	event_parsers.h \
	calibration_data.h \
	dircTH1D.h \
	utility_library.h \
	FileProperties.h \
	LinkDef.h
		rootcint -f $@ -c $(DEFAULT_INCLUDES) $(INCLUDES) $^


libDircObjs_la_SOURCES = \
	analysis.cpp \
	dirc_objects.cpp \
	particle_objects.cpp \
	dircTH1D.cpp \
	TrackRecons.cpp \
	dircTFile.cpp \
	utility_library.cpp \
	FileProperties.cpp \
	DircObjs_Dict.cpp

libDircObjs_la_LIBADD = \
$(ROOT_LIBS) \
$(NEWTON_LIB)

libDircObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = DircObjs.pc


clean-local:
	rm -f $(BUILT_SOURCES) *Dict*