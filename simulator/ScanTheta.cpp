#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Scan through Thetas
================================================================================================*/
void ScanTheta(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_theta, double ending_theta, double theta_step,
							TTree* tree, int input, Displayer Output)
{

	int i = 0;
	if (Output.Main == "yes")
	{
		TabToLevel(2); cout << "ScanTheta:\n";
	}

	double theta = beginning_theta;

	while(theta < ending_theta)
	{
		// param.Particle.Range_Phi(theta, theta);
		if (i == detector.Checker){ Output.Main = "yes"; Output.Trivial = "yes"; Output.Important = "yes"; }
		else {Output.Important = "no"; Output.Trivial = "no"; Output.Main = "no"; }
	  GenerateEvent(detector, particle_events, photon_events, param,
                tree, input, Output);
		FillTree(*tree, particle_events.back(), photon_events.back(), event_output, Output.Important);
		theta = theta + theta_step;
		if (Output.Specific == "yes")
		{
			TabToLevel(3); cout << "Particle theta = " << theta << endl;
		}
		i++;
	}



}