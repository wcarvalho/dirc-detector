#include "TMath.h"
#include <iostream>
#include <vector>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/simulator.h"

using namespace std;

/*================================================================================================
Fill ParticleEvent with default values
================================================================================================*/

void Fill_ParticleEvent(Detector d, ParticleEvent &event, ParticleParameters parameters, int num_particles, int input, string Output)
{
	int i;
	double pi = TMath::Pi();
  Particle particle;
  Random r;

  Output = "no";

  Introduce("Fill_ParticleEvent", Output);

	for (i = 0; i < num_particles; i++)
  {
  	event.Particles.push_back(particle);
  	event.Particles.back().Which = i;
		event.Particles.back().Theta = r.Uniform(parameters.Theta[0], parameters.Theta[1]);
		event.Particles.back().Phi = r.Uniform(parameters.Phi[0], parameters.Phi[1]);
  	event.Particles.back().UnitVector = Get_UnitVector(event.Particles.back().Theta, event.Particles.back().Phi);
		// event.Particles.back().EmissionTimes = Random_Emission_Times(particle, "zero", Output);
  	event.Particles.back().IncidentX = r.Uniform(d.Length*parameters.X[0], d.Length*parameters.X[1]);
  	event.Particles.back().IncidentY = r.Uniform(d.Width*parameters.Y[0], d.Width*parameters.Y[1]);
  	event.Particles.back().X = event.Particles.back().IncidentX;
  	event.Particles.back().Y = event.Particles.back().IncidentY;
  	event.Particles.back().ConeAngle = r.Uniform(parameters.Cone[0], parameters.Cone[1]);
  	r.Int(parameters.Photons[0], parameters.Photons[1], event.Particles.back().NumberofPhotons);
  	if (Output == "yes")
  	{
	  	TabToLevel(3); cout << "Which Particle = " << event.Particles.back().Which << endl;
	  	TabToLevel(4); cout << "Theta = " << event.Particles.back().Theta << endl;
	  	TabToLevel(4); cout << "UnitVector = "; Print_TVector(event.Particles.back().UnitVector);
	  	TabToLevel(4); cout << "IncidentX = " << event.Particles.back().IncidentX << endl;
	  	TabToLevel(4); cout << "IncidentY = " <<  event.Particles.back().IncidentY << endl;
	  	TabToLevel(4); cout << "X = " << event.Particles.back().X << endl;
	  	TabToLevel(4); cout << "Y = " <<  event.Particles.back().Y << endl;
  	}
  }

}