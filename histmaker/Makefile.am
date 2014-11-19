AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = histmaker

include_HEADERS = \
	../headers/histmaker.h \
	../headers/functions.h \
	../headers/file-properties.h


histmaker_LDADD = \
	$(ROOT_LIBS) \
	$(LIB)

histmaker_SOURCES = \
	main.cpp \
	cmdline.c \
	Print_Event.C \
	../functions/fixedLength.C \
	../functions/file-properties.cpp

cmdline.c: cmd.ggo
  gengetopt --input=cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*