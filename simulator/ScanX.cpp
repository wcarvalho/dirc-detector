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
void ScanX(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_x, double ending_x, double x_step,
							TTree* tree, int input, Displayer Output)
{

	int i;
	double x = beginning_x;

	while (x < ending_x)
	{
		ScanTheta(detector, particle_events, photon_events,
		          param, event_output,
		          0, 1, 1.0/20,
		          tree, input, Output);
		x += x_step;
	}


}