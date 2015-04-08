AUTOMAKE_OPTIONS = subdir-objects

H=../headers
bin_PROGRAMS = generator

include_HEADERS = \
	$(H)/generator.h \
	$(H)/functions.h \
	$(H)/myrandom.h

generator_LDADD = \
	$(ROOT_LIBS) \
	$(DIRC_LIB)

generator_SOURCES = \
	main.cpp \
	cmdline.cpp \
	generate_particle_data.cpp \
	project.cpp \
	TakeInParameters.cpp \
	generate.cpp \
##=====================================
##
##					Mutual functions
##
##=====================================
	../functions/random_numbers.cpp \
	../functions/TabToLevel.cpp

cmdline.c: cmdline.ggo
  gengetopt --input=cmdline.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*