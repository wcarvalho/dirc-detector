#include "TMath.h"
#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TPad.h"
#include "TH2D.h"
#include "TCanvas.h"
#include <iostream>
#include "dirc_objects.h"

using namespace std;
int main(int argc, char** argv)
{
	int i, j, k;
	string name;
	string title ;
  string hname;
  int pads;
	double pi = TMath::Pi();
	double theta_bin;

  GeneratorOut* event_output = 0;
	Reconstruction* reconstruction = 0;


	TFile f1("dirc_events.root", "read");
	TFile f2("ReconstructionData.root", "read");

	TTree *events = (TTree*)f1.Get("events");
	TTree *output = (TTree*)f2.Get("output");

	events -> SetBranchAddress("event_output", &event_output);
	output -> SetBranchAddress("reconstruction", &reconstruction);

  TCanvas c1_base("c1", "c1",1,1,2000,600);
  TPad pad_base("pad", "pad",.1,0.1,0.9,0.8);

  TCanvas* c1 = &c1_base;
  TPad* pad = &pad_base;

  pad->Draw();
  pad->cd();

  TH2D h(hname.c_str(), title.c_str(), 100, -pi, pi, 100, 0, pi/2);
 //  // cout << "event size = " << events->GetEntries() << endl;
  for (k = 0; k < 4; k++)
  {
		for (i = 0; i < events->GetEntries(); i++)
		{
		  events->GetEntry(i);
		  output->GetEntry(i); 
			// pads = event_output->Particles.size();
		  cout << "\n\n\n\n";
		  // cout << "Number of Pads: "<< pads << endl;
	  	h.Reset();
	  	// pad->Clear();
		  // pad->Divide(pads, 1, 0.01, 0.01);
		  	// pad->cd(k+1);
	  	// cout << "\tat pad " << k+1 << endl;

		  title.append("Particle Theta = ");
		  string title_theta = to_string(event_output->Particles.at(k).Theta);
		  title.append(title_theta);
	  	// cout << "\ttitle = " << title << endl;
		  string hwhich = to_string(i+k);
		  hname.append(hwhich);
	  	// cout << "\tname = " << title << endl;
	  	h.SetName(hname.c_str());
	  	h.SetTitle(title.c_str());
		  title.clear();
		  hname.clear();
		  // cout << "\tOne Theta is: " << reconstruction->Photons.at(k).at(20).Theta << endl;
		  for (j = 0; j < reconstruction->Photons.at(k).size(); j++)
		  {
		  	h.Fill(reconstruction->Photons.at(k).at(j).Phi, reconstruction->Photons.at(k).at(j).Theta);
		  	// cout << "\tx,y = " << reconstruction->Photons.at(k).at(j).Phi << ", "<< reconstruction->Photons.at(k).at(j).Theta << endl;
		  }
		  h.Draw("colz");

		  double max = h.GetBinContent(h.GetMaximumBin());
		 //  int max_bin = h.GetMaximumBin();
			h.GetYaxis()->GetBinCenter(0);
			for (j = 1; j < max; j++)
			{
				// cout << h.GetYaxis()->GetBinCenter(i) << endl;				
				if (h.GetYaxis()->GetBinCenter(j) > h.GetYaxis()->GetBinCenter(j-1))
				{
				theta_bin = h.GetYaxis()->GetBinCenter(j);
				}
			}
			cout << "theta_bin = " << theta_bin << endl;
		  // cout << "\t Drew\n";
			name.append("Graphs/Event_");
			string s = to_string(i);
			name.append(s);
			name.append("_Particle_");
			string s0 = to_string(k+1);			
			name.append(s0);
			name.append(".png");
			c1->Print(name.c_str());
			name.clear();
		  // exit(1);
		}
	}
}

