AUTOMAKE_OPTIONS = subdir-objects

commonAdds = \
$(ROOT_LIBS) \
$(DIRC_LIB)

commonSources = \
	cmdline.cpp \
	Simulate_PhotonPath.cpp \
	Simulate_ParticlePath.cpp \
	CheckForFlag.cpp \
	CheckAngel.cpp \
	Check_PhotonDirection.cpp \
	FillTree.cpp \
	Simulate.cpp \
	Pull_Data.cpp \
	Rotater.cpp

include_HEADERS = simulator.h \
	Rotater.h

lib_LTLIBRARIES = libSimObjs.la
libSimObjs_la_SOURCES = $(commonSources)
libSimObjs_la_LIBADD = $(commonAdds)
libSimObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = SimObjs.pc

bin_PROGRAMS = simulator
simulator_SOURCES = main.cpp
simulator_LDADD = libSimObjs.la
simulator_CFLAGS = $(AM_CFLAGS)


cmdline.c: sim-cmd.ggo
  gengetopt --input=sim-cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*