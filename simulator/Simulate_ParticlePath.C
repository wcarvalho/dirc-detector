#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectory for a single Particle of a single "Event"
================================================================================================*/

void Simulate_ParticlePath(Detector d, Particle particle, PhotonEvent &photon_event, string Output)
{
	Random r;
	double Path_length = 0.;
	double avg_d = 0.;
	int photons_released = 0;
	int avg_released = 0;

	if (Output == "yes"){
		TabToLevel(3); cout << "Simulate_ParticlePath:\n";
	}


	Simulate simPar(particle.Theta, particle.Phi);
	
	simPar.SetDim(d.Length, d.Width, d.Height);
	simPar.SetStart(particle.X, particle.Y, 0);
	simPar.DistancetoWalls( );
	simPar.WhichWall( );
	Path_length = simPar.WillTravel();

	avg_d = 1;
	avg_released = 1000;
	photons_released = r.Poisson(avg_released);

	int howmany = 0;
	while (simPar.Traveled < Path_length)
	{
		Release_Photons(simPar, photon_event, photons_released, particle.ConeAngle);
		simPar.TravelDistance(avg_d);
		howmany += photons_released;
	}
}