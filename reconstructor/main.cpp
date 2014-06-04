//====================================================
//
//                RECONSTRUCTOR
//
//====================================================

#include "../headers/reconstructor.h"
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }
	if (ai.new_given)
	{
	  system("exec ./../../generator/build/generator");
	  system("exec ./../../simulator/build/simulator");
	}

	int i=0, j=0;
  int input = 4;
  
  double pi = TMath::Pi();
  Displayer disp;
  Displayer disp_def;
  Displayer disp_yes;

  Reconstruction reconstruction;
	ReconstructionData data;
	reconstruction.Track.push_back(data);

  // pointers to data from ROOT File
  GeneratorOut *event_output = 0;

  TFile file("../../root_files/simulator.root", "read");
	TTree *events = (TTree*)file.Get("sim_out");
	events->SetBranchAddress("simEvent", &event_output);

	TFile file2("../../root_files/reconstructor.root", "recreate");
  TTree* tree = new TTree("output", "a tree of Reconstruction Data");
	tree->Branch("recEvent", &reconstruction);


  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nRECONSTRUCTOR\n";

	int checker = -1;
	disp.checker = checker;
  for (i = 0; i < events->GetEntries(); i++)
  {
  	// cout << "event " << i << endl;
  	if (i == disp.checker) {disp.Main = "yes"; disp.Action = "yes"; disp.Specific = "no"; disp.General = "yes"; disp.Data = "yes"; cout << "Checking" << endl;}
  	else { disp = disp_def; disp.checker = checker; }
	  events->GetEntry(i);
	  ReconstructEvent(reconstruction, event_output, disp);
	  tree->Fill();
	  if (i == disp.checker) { break; }
  }

  file2.Write();

  file.cd();
  file.Close();

  file2.cd();
  file2.Close();


}