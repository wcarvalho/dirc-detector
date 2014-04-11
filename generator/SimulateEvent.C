#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/

void SimulateEvent(Detector d, ParticleEvent &particle_event, PhotonEvent &photon_event, string Output)
{
	int i;

	if (Output == "yes")
	{		
		TabToLevel(2); cout << "SimulateEvent: " << particle_event.Event << endl;
	}

	for (i = 0; i < particle_event.Particles.size(); i++)
	{
		Simulate_ParticlePath(d, particle_event.Particles[i], photon_event, Output);
	}

	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		Simulate_PhotonPath(d, photon_event.Photons[i], particle_event.Particles[photon_event.Photons[i].WhichParticle], Output);
		CheckForFlag(photon_event, i, Output);
	}
}
