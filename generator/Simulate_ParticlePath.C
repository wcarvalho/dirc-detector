#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectory for a single Particle of a single "Event"
================================================================================================*/

void Simulate_ParticlePath(Detector d, Particle particle, PhotonEvent &photon_event, string Output)
{
	if (Output == "yes"){
		TabToLevel(3); cout << "Simulate_ParticlePath:\n";
	}
	Get_ParticleDistance(d, particle, Output);
	Get_ParticleWall(particle, Output);
	particle.EmissionTimes = Random_Emission_Times(particle, "random", Output);
	Push_Corresponding_Coordinates(particle, photon_event, Output);
}