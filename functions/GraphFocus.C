#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/
void GraphFocus(TCanvas *c1, TGraph*& focus, PhotonEvent photon_event, string Output)
{
	int i, k=0;
	int focus_size;

  if (Output == "yes")
  {
		TabToLevel(2); cout << "GraphFocus:\n";	
  }

	focus_size = Corresponding_Photons(photon_event, photon_event.Focus);
	focus->Set(0);
	focus->Set(focus_size);
	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		if (photon_event.Photons[i].WhichParticle == photon_event.Focus)
		{
			focus -> SetPoint(k, photon_event.Photons[k].Phi, photon_event.Photons[k].Theta);
			k++;
		}
	}
  focus -> SetMarkerStyle(7);
  focus -> SetMarkerColor(2);
  focus -> Draw("P");
  if (Output == "yes")
  {
	  focus -> Print();
  }

}