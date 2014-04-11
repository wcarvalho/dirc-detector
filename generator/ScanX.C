#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/generator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Scan through Thetas
================================================================================================*/
void ScanX(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              ParticleEvent default_ParticleEvent, GeneratorOut& event_output,
              double beginning_x, double ending_x, double x_step,
							TTree* tree, TGraph*& g1, TGraph*& g2, int input, string Output)
{

	int i;
	double x = beginning_x;

	while (x < ending_x)
	{
		default_ParticleEvent.Particles[0].X = x;
		ScanTheta(detector, particle_events, photon_events,
		          default_ParticleEvent, event_output,
		          0, 1, 1.0/20,
		          tree, g1, g2, input, Output);
		x += x_step;
	}


}