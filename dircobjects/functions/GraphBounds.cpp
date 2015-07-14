#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TMath.h"
#include "TGraph.h"
#include "simulator.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Simulate the trajectories for all of the Photons of a single "Event"
================================================================================================*/
void GraphBounds(TCanvas *c1, string Output)
{

	double pi = TMath::Pi();

  if (Output == "yes")
  {
		TabToLevel(2); cout << "GraphBounds:\n";
  }

	TGraph *graph1 = new TGraph(3);

	graph1 -> SetPoint(0, pi, 0);
	graph1 -> SetPoint(1, -pi, 0);
	graph1 -> SetPoint(2, 0, pi);


  graph1 -> SetMarkerStyle(7);
  graph1 -> SetMarkerColor(0);
  graph1 -> Draw("AP");
  if (Output == "yes")
  {
	  graph1 -> Print();
  }

}