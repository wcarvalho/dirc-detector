#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Fill Tree`
================================================================================================*/

void Pull_Photons(PhotonEvent photon_event, GeneratorOut& event_output, string Output)
{
	int i;
	PhotonOut photon;
	// Output = "yes";
	event_output.Photons.clear();
	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		event_output.Photons.push_back(photon);
		event_output.Photons.back().Theta = photon_event.Photons[i].Theta;
		event_output.Photons.back().Phi = photon_event.Photons[i].Phi;
		if (Output == "yes")
		{
			TabToLevel(3); cout << "Theta = " << event_output.Photons.back().Theta << endl;
			TabToLevel(3); cout << "Phi = " << event_output.Photons.back().Phi << endl;
		}
	}
}

void Pull_Particles(ParticleEvent particle_event, GeneratorOut& event_output, string Output)
{
	int j;
	ParticleOut particle;

	event_output.Particles.clear();
	for (j = 0; j < particle_event.Particles.size(); j++)
	{
		event_output.Particles.push_back(particle);
		event_output.Particles.back().Theta = particle_event.Particles[j].Theta;
		event_output.Particles.back().Phi = particle_event.Particles[j].Phi;
		event_output.Particles.back().X = particle_event.Particles[j].IncidentX;
		event_output.Particles.back().Y = particle_event.Particles[j].IncidentY;
		if (Output == "yes")
		{
			TabToLevel(3); cout << "Theta = " << event_output.Particles.back().Theta << endl;
			TabToLevel(3); cout << "Phi = " << event_output.Particles.back().Phi << endl;
		}
	}

}