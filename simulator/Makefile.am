AUTOMAKE_OPTIONS = subdir-objects

commonAdds = \
$(ROOT_LIBS) \
$(DIRC_LIB)

commonSources = \
	cmdline.cpp \
	find_critical_angle.cpp \
	PushBack_ParticleEvent.cpp \
	Fill_ParticleEvent.cpp \
	PushBack_PhotonEvent.cpp \
	SimulateEvent.cpp \
	Simulate_PhotonPath.cpp \
	Simulate_ParticlePath.cpp \
	CheckForFlag.cpp \
	Get_ParticleDistance.cpp \
	Find_Particle_X_Distance.cpp \
	Find_Particle_Y_Distance.cpp \
	Find_Particle_Z_Distance.cpp \
	Get_ParticleWall.cpp \
	Random_Emission_Times.cpp \
	Push_Corresponding_Coordinates.cpp \
	Get_ParticleCoordinate.cpp \
	Get_PhotonDistance.cpp \
	Find_Photon_X_Distance.cpp \
	Find_Photon_Y_Distance.cpp \
	Find_Photon_Z_Distance.cpp \
	Get_PhotonWall.cpp \
	CheckAngel.cpp \
	Check_PhotonDirection.cpp \
	Move_Photon.cpp \
	Change_Angle.cpp \
	GraphEvent.cpp \
	GenerateEvent.cpp \
	FillTree.cpp \
	ScanTheta.cpp \
	ScanX.cpp \
	ScanY.cpp \
	ScanPhi.cpp \
	Pull_Data.cpp \
	Release_Photons.cpp \
	Simulate.cpp \
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
	../functions/Rotater.cpp \
	../functions/returnEnd.cpp \
	../functions/addtoFile.cpp \
	../functions/file-properties.cpp

include_HEADERS = ../headers/simulator.h \
../headers/functions.h \
../headers/Parameters.h \
../headers/Simulate.h \
../headers/file-properties.h

lib_LTLIBRARIES = libSimObjs.la
libSimObjs_la_SOURCES = $(commonSources)
libSimObjs_la_LIBADD = $(commonAdds)
libSimObjs_la_LDFLAGS = -version-info 1:0:0

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = SimObjs.pc

bin_PROGRAMS = simulator
simulator_SOURCES = main.cpp
simulator_LDADD = libSimObjs.la
simulator_CFLAGS = $(AM_CFLAGS)


cmdline.c: sim-cmd.ggo
  gengetopt --input=sim-cmd.ggo --include-getopt

clear-local:
	rm -f $(BUILT_SOURCES) *Dict*