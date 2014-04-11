#include <iostream>
#include "dirc_objects.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Check if Photon failed simulation
================================================================================================*/


void CheckForFlag(PhotonEvent &photon_event, int &iterator, string Output)
{
	if (photon_event.Photons[iterator].Flag == 1)
	{
		if (Output == "yes")
		{
			TabToLevel(3); cout << "Deleting Photon " << photon_event.Photons[iterator].Which;
			cout << " from Particle " << photon_event.Photons[iterator].WhichParticle << endl;
		}
		photon_event.Photons.erase(photon_event.Photons.begin() + iterator);
		iterator = iterator - 1;
	}
}