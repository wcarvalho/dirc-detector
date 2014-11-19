AUTOMAKE_OPTIONS = subdir-objects

bin_PROGRAMS = simulator

include_HEADERS = ../headers/simulator.h \
../headers/functions.h \
../headers/Parameters.h \
../headers/file-properties.h

simulator_LDADD = \
$(ROOT_LIBS) \
$(DIRC_LIB)

simulator_SOURCES = \
	main.cpp \
	cmdline.c \
	find_critical_angle.C \
	PushBack_ParticleEvent.C \
	Fill_ParticleEvent.C \
	PushBack_PhotonEvent.C \
	SimulateEvent.C \
	Simulate_PhotonPath.C \
	Simulate_ParticlePath.C \
	CheckForFlag.C \
	Get_ParticleDistance.C \
	Find_Particle_X_Distance.C \
	Find_Particle_Y_Distance.C \
	Find_Particle_Z_Distance.C \
	Get_ParticleWall.C \
	Random_Emission_Times.C \
	Push_Corresponding_Coordinates.C \
	Get_ParticleCoordinate.C \
	Get_PhotonDistance.C \
	Find_Photon_X_Distance.C \
	Find_Photon_Y_Distance.C \
	Find_Photon_Z_Distance.C \
	Get_PhotonWall.C \
	CheckAngel.C \
	Check_PhotonDirection.C \
	Move_Photon.C \
	Change_Angle.C \
	GraphEvent.C \
	GenerateEvent.C \
	FillTree.C \
	ScanTheta.C \
	ScanX.C \
	ScanY.C \
	ScanPhi.C \
	Pull_Data.C \
	Release_Photons.C \
##=====================================
##
##					Mutual functions
##
##=====================================
	../functions/simulate_par.C \
	../functions/TabToLevel.C \
	../functions/Get_Photon_UnitVector.C \
	../functions/Get_Particle_UnitVector.C \
	../functions/Print_TVector.C \
	../functions/Introduce.C \
	../functions/create_2D_vector.C \
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
	../functions/Rotater.C \
	../functions/returnEnd.cpp \
	../functions/addtoFile.cpp \
	../functions/file-properties.cpp

cmdline.c: sim-cmd.ggo
  gengetopt --input=sim-cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*