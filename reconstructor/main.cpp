//====================================================
//
//                RECONSTRUCTOR
//
//====================================================

#include "../headers/reconstructor.h"

int main()
{
	int i=0, j=0;
  int input = 4;
  
  double pi = TMath::Pi();
  string Output;

  Reconstruction reconstruction;
  Reconstruction* reconstruction_p = &reconstruction;
	ReconstructionData data;
	reconstruction.Track.push_back(data);

	TH2D h1("h1","h1",100, -pi, pi, 100, 0, pi);
  // pointers to data from ROOT File
  GeneratorOut *event_output_in = 0;

  TCanvas c1_base("c1", "c1",1,1,2000,600);
  TCanvas* c1 = &c1_base;

  TFile file("../../dirc_events.root", "read");
	TTree *events = (TTree*)file.Get("events");
	events->SetBranchAddress("event_output", &event_output_in);

	TFile file2("../../ReconstructionData.root", "recreate");
  TTree* tree = new TTree("output", "a tree of Reconstruction Data");
	tree->Branch("reconstruction", &reconstruction_p);


  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
  
  Output = "yes";
  char name[100];
  for (i = 0; i < events->GetEntries(); i++)
  {
  	if (i == 1) {Output = "yes"; }
  	// if (i == 2) { break; }
  	else { Output = "no"; }
  	cout << i << endl;
	  events->GetEntry(i);
    GeneratorOut event_output_copy = *event_output_in;
    GeneratorOut* event_output = &event_output_copy;

	  ReconstructEvent(reconstruction, event_output, h1, Output);
	  tree->Fill();
  }

  file2.Write();

  file.cd();
  file.Close();

  file2.cd();
  file2.Close();
  cout << "done\n";


}