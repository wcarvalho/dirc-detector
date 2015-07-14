#include <iostream>
#include <vector>
#include <string>
#include "TMath.h"
#include "dirc_objects.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Find the number of associated with a particular particle
================================================================================================*/

int Photons_Rest(PhotonEvent photon_event, int focus)
{
	int i;
	int focus_size = 0;
	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		if (photon_event.Photons[i].WhichParticle != focus)
		{
			focus_size++;
		}
	}

	return focus_size;
}