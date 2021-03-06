#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "reconstructor.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event"
================================================================================================*/
void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, bool print)
{

	static int i=0, j=0;
	static vector<PhotonOut> pho;
	static double *pho_theta, *pho_phi;
	int num_photons = event_output->Photons.size();

	for (i = 0; i < num_photons; i++)
	{
		pho.clear(); // empty vector
		pho_theta = &event_output->Photons[i].Theta;
		// if (i == 149){
		// 	cout << i << "\t\tpho_theta = " << *pho_theta << endl;
		// 	print = true;
		// }
		// else print = false;
		pho_phi = &event_output->Photons[i].Phi;
		Reconstructed_Reflections(pho, *pho_theta, *pho_phi, print); // add photon with flipped z, y, or both
		for (j = 0; j < pho.size(); j++)
		{
			event_output->Photons.push_back(std::move(pho[j])); // append to end of list of photons
		}
	}

	reconstruction.Photons.clear();
	for (i = 0; i < event_output->Particles.size(); i++)
	{
		ReconstructTrack(reconstruction, event_output->Particles.at(i), event_output->Photons, false);
	}
	if (!(reconstruction.Photons.size() == 0) && print){
		cout << "\t" << reconstruction.Photons.size() << " reconstructions, each with ";
		cout << reconstruction.Photons.back().size() << " photons\n";
	}

}