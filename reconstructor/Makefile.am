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
ReconstructEvent.C \
ReconstructTrack.C \
CreateHistogram.C \
Reconstructed_Reflections.C \
histograms.cpp \
FindLostPhotons.C \
LookUpTable.cpp \
##=====================================
##
##					Mutual functions
##
##=====================================
../functions/TabToLevel.C \
../functions/Get_Photon_UnitVector.C \
../functions/Get_Particle_UnitVector.C \
../functions/Print_TVector.C \
../functions/Introduce.C \
../functions/create_2D_vector.C \
../functions/Fill_RotationMatrix.C \
../functions/Print_2D_Vector.C \
../functions/Rotation_Vector.C \
../functions/Rotated_Vector.C \
../functions/Update_Photon_ThetaPhi.C \
../functions/random_numbers.C \
../functions/Check_PhotonEvent.C \
../functions/Corresponding_Photons.C \
../functions/Photons_Rest.C \
../functions/Check_ParticleEvent.C \
../functions/GraphFocus.C \
../functions/GraphRest.C \
../functions/GraphBounds.C \
../functions/ExtractArray.C \
../functions/Print_Histogram.C \
../functions/Rotater.C \
../functions/Displayer.C \
../simulator/CheckAngel.C \
../simulator/Check_PhotonDirection.C \
../functions/simulate_par.C \
../functions/masses.C \
../functions/identifier.C \
../functions/min_map.C \
../functions/addtoFile.cpp \
../functions/returnEnd.cpp \
../functions/file-properties.cpp

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*