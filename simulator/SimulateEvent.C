#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/

void SimulateEvent(Detector d, ParticleEvent &particle_event, PhotonEvent &photon_event, Displayer Output)
{
	int i=0;
	int fails = 0;

	if (Output.Main == "yes")
	{		
		TabToLevel(2); cout << "SimulateEvent: " << particle_event.Event << endl;
	}
	for (i = 0; i < particle_event.Particles.size(); i++)
	{

		Simulate_ParticlePath(d, particle_event.Particles[i], photon_event, "no");
	}
  if(Output.Important == "yes"){ Check_PhotonEvent(photon_event); }

	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		Simulate_PhotonPath(d, photon_event.Photons[i], Output);
		if (photon_event.Photons[i].Flag == 1){ ++fails; }
		CheckForFlag(photon_event, i, Output.Trivial);
	}
	cout << "fails = " << fails << endl;
}
