AUTOMAKE_OPTIONS = subdir-objects

commonAdds = \
$(ROOT_LIBS) \
$(DIRC_LIB) \
$(SIM_LIB) \
$(NEWTON_LIB)

include_HEADERS = reconstructor.h

commonSources = LookUpTable.cpp \
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
rebinHistogram.cpp \
reconstruct_photons.cpp \
rotate_photons_into_particle_frame.cpp \
histogram_photon_angles.cpp \
getExpectedPhotonMap.cpp \
CalculateParticleFits.cpp \
reset_variables.cpp \
check_reconstructed_photons.cpp


lib_LTLIBRARIES = libReconstructionObjs.la
libReconstructionObjs_la_SOURCES = $(commonSources)
libReconstructionObjs_la_LIBADD = $(commonAdds)
libReconstructionObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = ReconstructionObjs.pc

bin_PROGRAMS = reconstructor
reconstructor_SOURCES = main.cpp
reconstructor_LDADD = libReconstructionObjs.la \
	$(commonAdds)
reconstructor_CFLAGS = $(AM_CFLAGS)

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*