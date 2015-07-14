#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "simulator.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/
void GraphRest(TCanvas *c1, TGraph*& rest, PhotonEvent photon_event, string Output)
{
	int i, k =0;
	int focus_size;

  if (Output == "yes")
  {
		TabToLevel(2); cout << "GraphRest:\n";
  }

	focus_size = Photons_Rest(photon_event, photon_event.Focus);
	rest->Set(0);
	rest->Set(focus_size);
	for (i = 0; i < photon_event.Photons.size(); i++)
	{
		if (photon_event.Photons[i].WhichParticle != photon_event.Focus)
		{
			rest -> SetPoint(k, photon_event.Photons[k].Phi, photon_event.Photons[k].Theta);
			k++;
		}
	}
  rest -> SetMarkerStyle(7);
  rest -> SetMarkerColor(1);
  rest -> Draw("P");
  if (Output == "yes")
  {
	  rest -> Print();
  }

}