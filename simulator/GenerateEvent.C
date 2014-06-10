#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TRandom.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Generate one "Event"
================================================================================================*/
void GenerateEvent(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
                   Parameter param, TTree* tree, int input, Displayer Output)
{

	int i=0, h=0;
	double *pho_theta, *pho_phi;
	Rotater r;
	PushBack_ParticleEvent(detector, particle_events, param, input, "no");
	
  if(Output.Important == "yes"){ Check_ParticleEvent(particle_events.back()); }

  PushBack_PhotonEvent(particle_events.back(), photon_events, input, "no");	
	
	for (h = 0; h < particle_events.back().Particles.size(); h++)
	{
		r.Feed_Particle(particle_events.back().Particles[h].Theta, particle_events.back().Particles[h].Phi);
		for(i = 0; i < photon_events.back().Photons.size(); i++)
		{
			pho_theta = &photon_events.back().Photons[i].Theta;
			pho_phi = &photon_events.back().Photons[i].Phi;
			if(photon_events.back().Photons[i].WhichParticle == h)
			{
				r.Rotate_Photon(*pho_theta, *pho_phi);
				photon_events.back().Photons[i].UnitVector = Get_UnitVector(*pho_theta, *pho_phi);
			}
		}
	}

  SimulateEvent(detector, particle_events.back(), photon_events.back(), Output);
	
}