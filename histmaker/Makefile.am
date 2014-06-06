AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = histmaker

include_HEADERS = \
	../headers/histmaker.h \
	../headers/functions.h

histmaker_LDADD = \
	$(ROOT_LIBS) \
	$(LIB)

histmaker_SOURCES = \
	main.cpp \
	cmdline.c \
	Print_Event.C \
	../functions/fixedLength.C

cmdline.c: cmd.ggo
  gengetopt --input=cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*