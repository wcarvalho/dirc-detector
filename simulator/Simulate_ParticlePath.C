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

void Simulate_ParticlePath(Detector d, Particle particle, PhotonEvent &photon_event, double avg_d, bool print)
{
	Random r;
	double Path_length = 0.;
	double avg_photons_released = 0.;
	double photons_released = 0.;
	int original_photon_count = 0;
	int howmany = 0;
	int steps = 100;

	Simulate simPar(particle.Theta, particle.Phi);

	simPar.SetDim(d.Length, d.Width, d.Height);
	simPar.SetStart(particle.X, particle.Y, 0);
	simPar.DistancetoWalls( );
	simPar.WhichWall( );
	Path_length = simPar.WillTravel();

	avg_photons_released = Path_length*particle.PhotonsPercm;
	photons_released = r.Poisson(avg_photons_released);

	double PhotonsPerStep = photons_released/steps;
	
	if (print) printf("Typcially %f photons\n", avg_photons_released);

	if (print){
		printf("\t\tphoton emissions at:\n");
	}
	vector<Photon> &numpho = photon_event.Photons;
	int numPho0 = numpho.size();
	
	for (unsigned int i = 0; i < steps; ++i)
	{
		Release_Photons(simPar, photon_event, PhotonsPerStep, particle.ConeAngle);
		// if (print) Check_PhotonEvent(photon_event);
		simPar.TravelDistance(Path_length/steps);
		if (print){
			printf("\t\t\tx = %f, y = %f, z = %f\n", simPar.coord[0], simPar.coord[1], simPar.coord[2]);
		}
		howmany = numpho.size() - numPho0;
	}
	if (print) printf("released %i photons\n", howmany);

}