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
void ScanTheta(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              ParticleEvent default_ParticleEvent, GeneratorOut& event_output,
              double beginning_theta, double ending_theta, double theta_step,
							TTree* tree, TGraph*& g1, TGraph*& g2, int input, string Output)
{

	int i = 0;
	if (Output == "yes")
	{
		TabToLevel(2); cout << "ScanTheta:\n";
	}

	double theta = beginning_theta;

	while(theta < ending_theta)
	{
		if (i == detector.Checker){ Output = "yes"; }
		else {Output = "no"; }
	  GenerateEvent(detector, particle_events, photon_events, 
	                theta, default_ParticleEvent.Particles[0].Phi,
	                default_ParticleEvent.Particles[0].X, default_ParticleEvent.Particles[0].Y,
	                default_ParticleEvent.NumberofParticles, default_ParticleEvent.Particles[0].NumberofPhotons, 5,
	                tree, g1, g2, input, Output);
		FillTree(tree, particle_events.back(), photon_events.back(), event_output, Output);
		theta = theta + theta_step;
		if (Output == "yes")
		{
			TabToLevel(3); cout << "theta = " << theta << endl;
		}
		i++;
	}



}