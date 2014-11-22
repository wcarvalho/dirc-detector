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

void FillTree(TTree &tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output, GeneratorOut* event_outputCopy, bool Append, bool quiet)
{
	Pull_Photons(photon_event, event_output, Output);
	
	// have Particles in Particle_event
	// have ParticleOuts in event_output
	
	vector<ParticleOut> &pars = event_output.Particles;
	vector<PhotonOut> &phos = event_output.Photons;
	
	Particle *Par;
	Photon *Pho;
 
 	if (!Append){
		pars.clear();
		phos.clear();
	}
	else{
		pars = event_outputCopy->Particles;
		phos = event_outputCopy->Photons;
	}

	if(!quiet) {
		cout << "\n\tphotons pre: " << phos.size() << endl;
		cout << "\tparticles pre: " << pars.size() << "\n\n";
	}

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
	if(!quiet){ 
		cout << "\tphotons post: " << phos.size() << endl;
		cout << "\tparticles post: " << pars.size() << "\n";
	}
	tree.Fill();

}