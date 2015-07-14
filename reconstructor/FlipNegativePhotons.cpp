#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "reconstructor.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Change all photons so that they have hopefully* had an "even" number of reflections
================================================================================================*/
void FlipNegativePhotons(GeneratorOut*& event_output, Displayer &d)
{

	int i;

	if (d.Specific != "no")
	{
		TabToLevel(2); cout << "FlipNegativePhotons:\n";
	}

	for (i = 0; i < event_output->Photons.size(); i++)
	{
		FlipNegativePhoton(event_output->Photons.at(i), d);
	}

}

void FlipNegativePhoton(PhotonOut &photon, Displayer &d)
{
	double pi = TMath::Pi();
	TVector3 pho = Get_UnitVector(photon.Theta, photon.Phi);
	// cout << "d.Data = " << d.Data << endl;
	if (photon.Theta > pi/2 )
	{
		if (d.Specific != "no")
		{
			TabToLevel(3); cout << "Theta = " << photon.Theta << "\t->\t";
		}
		photon.Theta = pi - photon.Theta;
		pho.SetZ(-pho.Z());
		// photon.Theta = pho.Theta();
		// photon.Phi = pho.Phi();
		if (d.Specific != "no")
		{
			cout << photon.Theta << " vs. pi/2 = " << pi/2 << endl;
		}
	}



}