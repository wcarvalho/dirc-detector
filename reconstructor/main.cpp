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
  bool print = false;
	int checker = -1;
	if (ai.verbose_given) print = true;
	if (ai.event_given) checker = ai.event_arg;

  Reconstruction reconstruction;
	ReconstructionData data;
	reconstruction.Track.push_back(data);

  // pointers to data from ROOT File
  GeneratorOut *event_output = 0;

  string rf = "../../root_files/simulator.root";
  string wf = "../../root_files/reconstructor.root"; 

  TFile file(rf.c_str(), "read");
	TTree *events = (TTree*)file.Get("sim_out");
	events->SetBranchAddress("simEvent", &event_output);

	TFile file2(wf.c_str(), "recreate");
  TTree* tree = new TTree("output", "a tree of Reconstruction Data");
	tree->Branch("recEvent", &reconstruction);


  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nRECONSTRUCTOR\n";

	disp.checker = checker;
  for (i = 0; i < events->GetEntries(); i++)
  {
  	// cout << "event " << i << endl;
  	if (print == true)
  	{
			cout << "Event " << i << "\n";
  		disp.Main = "yes"; disp.Action = "yes"; disp.Specific = "no"; disp.General = "yes"; disp.Data = "yes"; 
  	}
  	if (i != checker) disp = disp_def;
	  events->GetEntry(i);
	  ReconstructEvent(reconstruction, event_output, disp);
	  if ((print == true) && (reconstruction.Photons.size() == 0)) 
	  	printf("\tEvent had no reconstructions");
	  tree->Fill();
  }

  file2.Write();

  file.cd();
  file.Close();

  file2.cd();
  file2.Close();

  cout << "file: " << wf << endl;
  return 0;
}