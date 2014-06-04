AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = printer

include_HEADERS = ../headers/printer.h \
../headers/functions.h

printer_LDADD = $(ROOT_LIBS) \
$(LIB)

printer_SOURCES = main.cpp \
cmdline.c \
Print_EventTH2D.C \
classes.C \
../functions/fixedLength.C

cmdline.c: cmd.ggo
  gengetopt --input=cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*