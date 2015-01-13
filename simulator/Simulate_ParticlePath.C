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

void Simulate_ParticlePath(Detector d, Particle &particle, int parnum, PhotonEvent &photon_event, double avg_d, bool print)
{
	Random r;
	double Path_length = 0.;
	double avg_photons_released = 0.;
	double photons_released = 0.;

	Simulate simPar(particle.Theta, particle.Phi);

	simPar.SetDim(d.Length, d.Width, d.Height);
	simPar.SetStart(particle.X, particle.Y, 0);
	simPar.DistancetoWalls(print);
	simPar.WhichWall(print);
	Path_length = simPar.WillTravel();
	
	if (print) cout << "\tTravling distance: " << Path_length << endl;
	if (print) cout << "\tPhotonsPercm: " << particle.PhotonsPercm << endl;

	avg_photons_released = Path_length*particle.PhotonsPercm;
	photons_released = r.Poisson(avg_photons_released);
	
	if (print) cout << "\tReleasing " << photons_released << " photons\n";
	print = false; //TEMPORARY 
	particle.nPhotonsPassed = photons_released;

	vector<Photon> &numpho = photon_event.Photons;
	int numPho0 = numpho.size();
	
	for (unsigned int i = 0; i < photons_released; ++i)
	{
		simPar.SetStart(particle.X, particle.Y, 0);
		double distance = r.Uniform(Path_length);

		simPar.TravelDistance(distance, print);

		Photon P(particle.ConeAngle, r.Uniform(2*TMath::Pi()));
		P.X = simPar.coord[0];
		P.Y = simPar.coord[1];
		P.Z = simPar.coord[2];
		P.WhichParticle = parnum;
		photon_event.Photons.push_back(P);

		
	}
}