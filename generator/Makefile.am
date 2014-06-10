AUTOMAKE_OPTIONS = subdir-objects

H=../headers
bin_PROGRAMS = generator

include_HEADERS = \
	$(H)/generator.h \
	$(H)/functions.h \
	$(H)/random.h \
	$(H)/Simulate.h

generator_LDADD = \
	$(ROOT_LIBS) \
	$(DIRC_LIB)

generator_SOURCES = \
	main.cpp \
	cmdline.c \
	generate_particle_data.C \
	generate_event_data.C \
	project.cpp \
##=====================================
##
##					Mutual functions
##
##=====================================
	../functions/random_numbers.C \
	../functions/TabToLevel.C \
	../functions/simulate_par.C

cmdline.c: cmdline.ggo
  gengetopt --input=cmdline.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*