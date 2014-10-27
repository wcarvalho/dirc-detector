AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = fitter

include_HEADERS = \
	../headers/fitter.h \
	../headers/functions.h \
	../headers/Simulate.h

fitter_LDADD = \
	$(ROOT_LIBS) -lSpectrum \
	$(DIRC_LIB)

fitter_SOURCES = \
	main.cpp \
	cmdline.c \
	FindLostPhotons.C \
##=====================================
##
##					Mutual functions
##
##=====================================
	../simulator/CheckAngel.C \
	../simulator/Check_PhotonDirection.C \
	../functions/Rotater.C \
	../functions/TabToLevel.C \
	../functions/simulate_par.C \
	../functions/masses.C \
	../functions/identifier.C \ 
	../functions/min_map.C \
	../functions/addtoFile.cpp \
	../functions/returnEnd.cpp

cmdline.c: fit-cmd.ggo
  gengetopt --input=fit-cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*