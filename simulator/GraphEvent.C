#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/
void GraphEvent(TGraph*& focus, TGraph*& rest, PhotonEvent photon_event, string Output)
{

  TCanvas c1_base("c1", "c1",1,1,2000,600);
  TCanvas* c1 = &c1_base;

	if (photon_event.Photons.size() == 0)
	{
		return;
	}

	if (Output == "yes")
	{
		TabToLevel(2); cout << "GraphEvent:\n";
	}
	GraphBounds(c1, Output);
	GraphRest(c1, rest, photon_event, Output);
	GraphFocus(c1, focus, photon_event, Output);
	
	if (Output == "yes")
	{
	  c1->Print("../../test.png");
	}
}