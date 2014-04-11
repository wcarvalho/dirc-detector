#include "TMath.h"
#include <iostream>
#include <vector>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/generator.h"

using namespace std;

/*================================================================================================
Fill ParticleEvent with default values
================================================================================================*/

void Fill_ParticleEvent(Detector d, ParticleEvent &event, int num_particles, int input, string Output)
{
	int i;
	double incident_X = d.Length/2;
	double incident_Y = d.Width/2;
	double pi = TMath::Pi();
  Particle particle;

  Output = "no";

  Introduce("Fill_ParticleEvent", Output);

	for (i = 0; i < num_particles; i++)
  {
  	event.Particles.push_back(particle);
  	event.Particles.back().Which = i;
		event.Particles.back().Theta = RandomNumberBetween(0, pi/4, input+i);
  	Get_Particle_UnitVector(event.Particles.back());
		event.Particles.back().EmissionTimes = Random_Emission_Times(particle, "zero", Output);
  	if (event.Particles.back().Which == event.Focus)
  	{
	  	event.Particles.back().IncidentX = incident_X;
	  	event.Particles.back().IncidentY = incident_Y;

  	}
  	else
  	{
	  	event.Particles.back().IncidentX = RandomNumberBetween(d.Length*.1, d.Length*.9, input+i);
	  	event.Particles.back().IncidentY = RandomNumberBetween(d.Width*.1, d.Width*.9, input+i+1);
  	}
  	event.Particles.back().X = event.Particles.back().IncidentX;
  	event.Particles.back().Y = event.Particles.back().IncidentY;
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