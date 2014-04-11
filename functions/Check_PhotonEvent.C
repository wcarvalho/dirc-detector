#include <iostream>
#include <vector>
#include <string>
#include "TMath.h"
#include "dirc_objects.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Check Values of a Photon Event
================================================================================================*/

void Check_PhotonEvent(PhotonEvent event)
{
	int i;
	TabToLevel(2); cout << "PhotonEvent " << event.Event << " with focus " << event.Focus <<":\n";
	TabToLevel(3); printf
	("Particle \t|Phi \t\t|Theta \t\t|Time \t\t|Reflections \t|X \t\t|Y \t\t|Z \t\t|\n");
	for(i = 0; i < event.Photons.size(); i++)
	{
  	TabToLevel(3); printf
  	("%i \t\t|%f \t|%f \t|%f \t|%i \t\t|%f \t|%f \t|%f \t|\n",
  	event.Photons[i].WhichParticle, 
  	event.Photons[i].Phi, event.Photons[i].Theta,
  	event.Photons[i].Time_Traveled, event.Photons[i].Reflections,
  	event.Photons[i].X, event.Photons[i].Y, event.Photons[i].Z);
	}
}