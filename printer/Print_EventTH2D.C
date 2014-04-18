#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "../headers/printer.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event"
================================================================================================*/

void Print_EventTH2D(Reconstruction* reconstruction, Printer printer, Displayer display)
{

	int j;
	string name;
	string title;
	double pi = TMath::Pi();


	TCanvas c("c", "c", 1,1,800,800);
  TH2D h(name.c_str(), title.c_str(), 100, -pi, pi, 100, 0, pi/2);

	string hwhich = to_string(printer.which);
	name.append(hwhich);


	h.SetName(name.c_str());
	h.SetTitle(printer.Hist_Title.c_str());
		
  for (j = 0; j < reconstruction->Photons.at(printer.which).size(); j++)
  {
  	h.Fill(reconstruction->Photons.at(printer.which).at(j).Phi, reconstruction->Photons.at(printer.which).at(j).Theta);
  	// cout << "\tx,y = " << reconstruction->Photons.at(k).at(j).Phi << ", "<< reconstruction->Photons.at(k).at(j).Theta << endl;
  }
  h.Draw("colz");

	printer.Name.append("_2DHist.png");


	c.Print(printer.Name.c_str());  
}

void Print_EventTH1D(Reconstruction* reconstruction, Printer printer, Displayer display)
{

	int j;
	string name;
	string title;
	double pi = TMath::Pi();


	TCanvas c("c", "c", 1,1,800,800);
  TH1D h(name.c_str(), title.c_str(), 100, -pi, pi);

	string hwhich = to_string(printer.which);
	name.append(hwhich);


	h.SetName(name.c_str());
	h.SetTitle(printer.Hist_Title.c_str());
		
  for (j = 0; j < reconstruction->Photons.at(printer.which).size(); j++)
  {
  	h.Fill(reconstruction->Photons.at(printer.which).at(j).Theta);
  	// cout << "\tx,y = " << reconstruction->Photons.at(k).at(j).Phi << ", "<< reconstruction->Photons.at(k).at(j).Theta << endl;
  }
  h.Draw();

	printer.Name.append("_1DHist.png");


	c.Print(printer.Name.c_str());  
}
