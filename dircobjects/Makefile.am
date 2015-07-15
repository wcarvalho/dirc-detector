AUTOMAKE_OPTIONS = subdir-objects

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
	calibration_data.h \
	functions.h \
	Parameters.h \
	fitter.h \
	myrandom.h \
	dircComparisons.h \
	dircTFile.h \
	dirc_io.h


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
	DircObjs_Dict.cpp \
	functions/TabToLevel.cpp \
	functions/Get_Photon_UnitVector.cpp \
	functions/Get_Particle_UnitVector.cpp \
	functions/Print_TVector.cpp \
	functions/create_2D_vector.cpp \
	functions/Print_2D_Vector.cpp \
	functions/Rotation_Vector.cpp \
	functions/Rotated_Vector.cpp \
	functions/Update_Photon_ThetaPhi.cpp \
	functions/random_numbers.cpp \
	functions/Photons_Rest.cpp \
	functions/returnEnd.cpp \
	functions/addtoFile.cpp \
	functions/Introduce.cpp \
	functions/Fill_RotationMatrix.cpp \
	functions/min_map.cpp


libDircObjs_la_LIBADD = \
$(ROOT_LIBS) \
$(NEWTON_LIB)

libDircObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = DircObjs.pc


clean-local:
	rm -f $(BUILT_SOURCES) *Dict*