#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/generator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Generate one "Event"
================================================================================================*/
void GenerateEvent(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
                   double Particle_Theta, double Particle_Phi, double Particle_X, double Particle_Y,
                   int num_particles, int num_photons, int num_emission,
									 TTree* tree, TGraph*& g1, TGraph*& g2, int input, string Output)
{
	// Output = "yes";

	PushBack_ParticleEvent(detector, particle_events, num_particles, input, Output);
	particle_events.back().Particles[0].Theta = Particle_Theta;
	particle_events.back().Particles[0].Phi = Particle_Phi;
	particle_events.back().Particles[0].X = Particle_X;
	particle_events.back().Particles[0].Y = Particle_Y;
	particle_events.back().Particles[0].Emissions = num_emission;
	particle_events.back().NumberofParticles = num_particles;
  Get_Particle_UnitVector(particle_events.back().Particles[0]);

 
  PushBack_PhotonEvent(particle_events.back(), photon_events, num_photons, input, Output);	
	Rotate_Photons(detector, particle_events.back(), photon_events.back(), Output);

  SimulateEvent(detector, particle_events.back(), photon_events.back(), Output);
	
	Check_ParticleEvent(particle_events.back());
	// Check_PhotonEvent(photon_events.back());
	// exit(1);

	GraphEvent(g1, g2, photon_events.back(), Output);
}