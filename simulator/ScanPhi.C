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
void ScanPhi(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_phi, double ending_phi, double phi_step,
							TTree* tree, int input, Displayer Output)
{

	int i;
	double phi = beginning_phi;

	while (phi < ending_phi)
	{
		ScanTheta(detector, particle_events, photon_events,
		          param, event_output,
		          0, 1, 1.0/20,
		          tree, input, Output);
		phi += phi_step;
	}


}