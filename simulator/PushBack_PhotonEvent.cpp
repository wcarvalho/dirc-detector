#include <iostream>
#include <vector>
#include <string>
#include "TMath.h"
#include "dirc_objects.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Push Back new Event (with parameters) into photon_events and particle_events
================================================================================================*/

void PushBack_PhotonEvent(ParticleEvent &particle_event, vector<PhotonEvent> &photon_events, int input, string Output)
{

	int i, j;
	double pi = TMath::Pi();

	Random r;
  PhotonEvent photon_event;
  photon_events.push_back(photon_event);

  photon_event.Focus = particle_event.Focus;

  Photon photon;
  Introduce("PushBack_PhotonEvent", Output);

  for (i = 0; i < particle_event.Particles.size(); i++)
  {
  	photon_events.back().NumberofPhotons = photon_events.back().NumberofPhotons + particle_event.Particles[i].NumberofPhotons;
  	for (j = 0; j < particle_event.Particles[i].NumberofPhotons; j++)
	  {
	  	photon_events.back().Photons.push_back(photon);
	  	photon_events.back().Photons.back().WhichParticle = i;
	  	photon_events.back().Photons.back().Which = j;
	  	photon_events.back().Photons.back().Theta = particle_event.Particles[i].ConeAngle;
	  	photon_events.back().Photons.back().Phi = r.Uniform(0, 2*pi);
  		photon_events.back().Photons.back().UnitVector = Get_UnitVector(photon_events.back().Photons.back().Theta, photon_events.back().Photons.back().Phi);
  		photon_events.back().Event = particle_event.Event;
	  	if (Output == "yes")
	  	{
		  	TabToLevel(3); cout << "Which Photon = " << photon_events.back().Photons.back().Which << endl;
		  	TabToLevel(4); cout << "WhichParticle = " << photon_events.back().Photons.back().WhichParticle << endl;
		  	TabToLevel(4); cout << "Theta = " << photon_events.back().Photons.back().Theta << endl;
		  	TabToLevel(4); cout << "Phi = " << photon_events.back().Photons.back().Phi << endl;
		  	TabToLevel(4); cout << "UnitVector = "; Print_TVector(photon_events.back().Photons.back().UnitVector);
	  	}
	  }
  }


}
