#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "TLatex.h"
#include "../headers/histmaker.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct one "Event"
================================================================================================*/

void Printer::PrintTH1D(unsigned int ev)
{
	TCanvas c("c", "c", 1,1,800,500);
	TH1D &h1 = Hists1D.at(ev);
  h1.SetXTitle("Theta");
  h1.SetYTitle("Entries");
  h1.Draw();
  
  string temp_f = filename;
  temp_f.append("_1D.png");
	c.Print(temp_f.c_str());  

}

void Printer::PrintTH2D(unsigned int ev)
{
	TCanvas c("c", "c", 1,1,800,500);
	TH2D &h2 = Hists2D.at(ev);
  h2.SetXTitle("Phi");
  h2.SetYTitle("Theta");
  h2.SetStats(0);
  h2.Draw("colz");
  
  string temp_f = filename;
  temp_f.append("_2D.png");
	c.Print(temp_f.c_str());  

}