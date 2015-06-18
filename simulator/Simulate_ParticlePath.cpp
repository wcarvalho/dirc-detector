#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectory for a single Particle of a single "Event"
================================================================================================*/

void Simulate_ParticlePath(Detector const& d, Particle &particle, int parnum, PhotonEvent &photon_event, int& seed, bool print)
{
	static Random r;
	// cout << "Seed = " << seed << endl;
	r.SetSeed(seed); if (seed > 0) ++seed;
	double Path_length = 0.;
	double avg_photons_released = 0.;
	double photons_released = 0.;

	double& particle_time_of_flight = particle.Time_Traveled;
	Simulate simPar(particle.Theta, particle.Phi);

	simPar.SetDim(d.Length, d.Width, d.Height);
	simPar.SetStart(particle.X, particle.Y, particle.Z);
	simPar.SetVelocity(particle.Beta);
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


	double time_traveled_so_far = particle.Time_Traveled;
	for (unsigned int i = 0; i < photons_released; ++i)
	{
		simPar.SetStart(particle.X, particle.Y, particle.Z);
		simPar.setTime(time_traveled_so_far);
		double distance = r.Uniform(Path_length);

		simPar.TravelDistance(distance, print);

		static Photon P(0,0);
		static double theta;
		static double phi;
		static double photon_time; photon_time = 0.;
		static double particle_time_of_flight_in_dirc;
		theta = particle.ConeAngle;
		phi = r.Uniform(2*TMath::Pi());
		P.SetAngle(theta, phi);
		P.X = simPar.coord[0];
		P.Y = simPar.coord[1];
		P.Z = simPar.coord[2];
		P.WhichParticle = parnum;

		particle_time_of_flight_in_dirc = distance/(particle.Beta/30);

		photon_time += particle_time_of_flight_in_dirc;
		photon_time += particle_time_of_flight;
		photon_time += simPar.GetTimeTraveled();

		P.Time_Traveled = photon_time;
		photon_event.Photons.push_back(P);
	}
}
