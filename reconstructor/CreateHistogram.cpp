#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event" in the frame of one of the "tracks"
================================================================================================*/

void CreateHistogram(TH2D& h1, vector<PhotonOut> photons, string Output)
{

	if (Output == "yes")
	{
		TabToLevel(2); cout << "CreateHistogram\n";
	}

	double x;
	int i;
  TCanvas c1_base("c1", "c1",1,1,2000,600);
  TCanvas* c1 = &c1_base;

  h1.Reset();

  for (i = 0; i<photons.size(); i++)
	{
	  h1.Fill(photons.at(i).Phi, photons.at(i).Theta);
	}
	h1.Draw("colz");
	if (Output == "yes")
	{
		c1->Print("test.png");
	}
}