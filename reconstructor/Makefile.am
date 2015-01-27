AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = reconstructor

include_HEADERS = ../headers/reconstructor.h \
../headers/functions.h \
../headers/Simulate.h \
../headers/file-properties.h


reconstructor_LDADD = \
$(ROOT_LIBS) \
$(DIRC_LIB) \
$(UTIL_LIB) \
$(NEWTON_LIB)

reconstructor_SOURCES = main.cpp \
cmdline.cpp \
reconstructor.cpp \
ReconstructEvent.cpp \
ReconstructTrack.cpp \
CreateHistogram.cpp \
Reconstructed_Reflections.cpp \
histograms.cpp \
FindLostPhotons.cpp \
LookUpTable.cpp \
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
../functions/simulate_par.cpp \
../functions/masses.cpp \
../functions/min_map.cpp \
../functions/addtoFile.cpp \
../functions/returnEnd.cpp \
../functions/file-properties.cpp

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*