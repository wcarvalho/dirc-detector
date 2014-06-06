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

void FillTree(TTree &tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output)
{
	Pull_Photons(photon_event, event_output, Output);
	
	// have Particles in Particle_event
	// have ParticleOuts in event_output
	
	vector<ParticleOut> &pars = event_output.Particles;
	vector<PhotonOut> &phos = event_output.Photons;
	
	Particle *Par;
	Photon *Pho;

	pars.clear();
	phos.clear();

	for (unsigned int par = 0; par < particle_event.Particles.size(); par++)
	{
		Par = &particle_event.Particles.at(par);
		pars.push_back(*Par);
	}
	
	for (unsigned int pho = 0; pho < photon_event.Photons.size(); pho++)
	{
		Pho = &photon_event.Photons.at(pho);
		phos.push_back(*Pho);
	}
	
	
	tree.Fill();

}