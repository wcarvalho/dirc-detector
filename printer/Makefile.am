AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = printer

include_HEADERS = ../headers/printer.h \
../headers/functions.h

printer_LDADD = $(ROOT_LIBS) \
$(LIB)

printer_SOURCES = main.cpp \
Print_EventTH2D.C \
classes.C \
../functions/fixedLength.C



clear-local:
	rm -f $(BUILT_SOURCES) *Dict*