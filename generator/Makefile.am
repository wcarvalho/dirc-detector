AUTOMAKE_OPTIONS = subdir-objects

commonAdds = \
	$(ROOT_LIBS) \
	$(DIRC_LIB) \
	$(SIM_LIB)

commonSources = \
	cmdline.cpp \
	gParticle.cpp \
	project.cpp \
	TakeInParameters.cpp \
	generate.cpp

include_HEADERS = \
	generator.h

lib_LTLIBRARIES = libGenObjs.la
libGenObjs_la_SOURCES = $(commonSources)
libGenObjs_la_LIBADD = $(commonAdds)
libGenObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = GenObjs.pc

bin_PROGRAMS = generator
generator_SOURCES = main.cpp
generator_LDADD = libGenObjs.la \
	$(commonAdds)
generator_CFLAGS = $(AM_CFLAGS)

cmdline.c: cmdline.ggo
  gengetopt --input=cmdline.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*