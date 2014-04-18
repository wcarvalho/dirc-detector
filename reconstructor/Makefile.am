AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = reconstructor

include_HEADERS = ../headers/reconstructor.h \
../headers/functions.h

reconstructor_LDADD = $(ROOT_LIBS) \
$(LIB)

reconstructor_SOURCES = main.cpp \
ReconstructEvent.C \
ReconstructTrack.C \
FlipNegativePhotons.C \
FlipNegativePhoton.C \
CreateHistogram.C \
##=====================================
##
##					Mutual functions
##
##=====================================
../functions/TabToLevel.C \
../functions/RandomNumberBetween.C \
../functions/Get_Photon_UnitVector.C \
../functions/Get_Particle_UnitVector.C \
../functions/Print_TVector.C \
../functions/Rotate_Photons.C \
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
../functions/Print_Histogram.C

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*