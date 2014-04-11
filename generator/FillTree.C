#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Fill Tree`
================================================================================================*/

void FillTree(TTree *tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output)
{
	int i;
	Pull_Photons(photon_event, event_output, Output);
	Pull_Particles(particle_event, event_output, Output);
	tree -> Fill();
	cout << particle_event.Event << endl;
  // tree -> Print();

}