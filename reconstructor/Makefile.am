AUTOMAKE_OPTIONS = subdir-objects

commonAdds = \
$(ROOT_LIBS) \
$(DIRC_LIB) \
$(SIM_LIB) \
$(UTIL_LIB) \
$(NEWTON_LIB) \
$(OPENCV_LIB)

include_HEADERS = reconstructor.h \
../headers/functions.h \
../headers/fitter.h \
../headers/myrandom.h \
../headers/file-properties.h

commonSources = LookUpTable.cpp \
CalculateParticleFits.cpp \
cmdline.cpp \
reconstructor.cpp \
ReconstructEvent.cpp \
ReconstructTrack.cpp \
CreateHistogram.cpp \
Reconstructed_Reflections.cpp \
histograms.cpp \
RiemannSum.cpp \
FitGaussianPlusConstant.cpp \
IndexPhotons.cpp \
CreateReducedHistogram.cpp \
##=====================================
##
##					Mutual functions
##
##=====================================
../functions/TabToLevel.cpp \
../functions/Get_Photon_UnitVector.cpp \
../functions/Get_Particle_UnitVector.cpp \
../functions/Print_TVector.cpp \
../functions/Introduce.cpp \
../functions/create_2D_vector.cpp \
../functions/Fill_RotationMatrix.cpp \
../functions/Print_2D_Vector.cpp \
../functions/Rotation_Vector.cpp \
../functions/Rotated_Vector.cpp \
../functions/Update_Photon_ThetaPhi.cpp \
../functions/random_numbers.cpp \
../functions/Check_PhotonEvent.cpp \
../functions/Corresponding_Photons.cpp \
../functions/Photons_Rest.cpp \
../functions/Check_ParticleEvent.cpp \
../functions/GraphFocus.cpp \
../functions/GraphRest.cpp \
../functions/GraphBounds.cpp \
../functions/ExtractArray.cpp \
../functions/Print_Histogram.cpp \
../functions/Rotater.cpp \
../functions/Displayer.cpp \
../simulator/CheckAngel.cpp \
../simulator/Check_PhotonDirection.cpp \
../functions/masses.cpp \
../functions/min_map.cpp \
../functions/addtoFile.cpp \
../functions/returnEnd.cpp \
../functions/file-properties.cpp

lib_LTLIBRARIES = libReconstructionObjs.la
libReconstructionObjs_la_SOURCES = $(commonSources)
libReconstructionObjs_la_LIBADD = $(commonAdds)
libReconstructionObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ReconstructionObjs.pc

bin_PROGRAMS = reconstructor
reconstructor_SOURCES = main.cpp
reconstructor_LDADD = libReconstructionObjs.la
reconstructor_CFLAGS = $(AM_CFLAGS)

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*