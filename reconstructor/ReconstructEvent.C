#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "../headers/reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event"
================================================================================================*/
void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, TH2D& h1, string Output)
{

	int i;
	
	FlipNegativePhotons(event_output, Output);
	reconstruction.Photons.clear();
	for (i = 0; i < event_output->Particles.size(); i++)
	{
		// cout <<"\t\tTheta = "<< event_output->Particles.at(i).Theta << endl;
		ReconstructTrack(reconstruction, event_output->Particles.at(i), event_output->Photons, h1, Output);
		// cout << "\t\t\toutter vector size = " << reconstruction.Photons.size() << endl;
		// cout << "\t\t\tinner vector size = " << reconstruction.Photons.back().size() << endl;
	}

}