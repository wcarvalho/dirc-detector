#include <iostream>
#include <string>
#include "TMath.h"
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Change photon so that it hopefully* has an "even" number of reflections
================================================================================================*/
void FlipNegativePhoton(PhotonOut &photon, string Output)
{
	Output = "no";
	double pi = TMath::Pi();

	if (photon.Theta > pi/2 )
	{
		if (Output == "yes")
		{
			TabToLevel(3); cout << "Theta = " << photon.Theta << "\t->\t";
		}
		photon.Theta = pi - photon.Theta;
		if (Output == "yes")
		{
			cout << photon.Theta << " vs. pi/2 = " << pi/2 << endl;
		}
	}

}