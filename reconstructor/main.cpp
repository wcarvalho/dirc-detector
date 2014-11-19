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

	int i=0, j=0;
  int input = 4;
  int beginning = 0;
  int last = 0;
  bool modified = ai.modified_particle_info_given;
  
  double pi = TMath::Pi();
  
  Displayer disp;
  Displayer disp_def;
  bool print = ai.verbose_given;
	int checker = -1;
  string rf_default = ai.input_arg;
  string wf_default = "reconstructor.root"; 
	string rf = rf_default;
  string wf = wf_default;
  string modified_eventoutput_file = "modified_particle_info.root";

  FileProperties readf_prop(rf);
  string directory = readf_prop.directory;

	if (ai.event_given) checker = ai.event_arg;
  if (ai.last_given) last = ai.last_arg;
  if (modified)
  	modified_eventoutput_file = ai.modified_particle_info_arg;

  if (ai.write_file_given) wf = ai.write_file_arg;

	if(ai.Directory_given) directory = ai.Directory_arg;
  readf_prop.appendFileToDirectory(directory, wf);
  readf_prop.appendFileToDirectory(directory, modified_eventoutput_file);


  Reconstruction reconstruction;
	ReconstructionData data;
	reconstruction.Track.push_back(data);

  // pointers to data from ROOT File
  GeneratorOut *event_output = 0;
	GeneratorOut *modified_event_output = 0;

	// cout << "readfile = " << rf <<x
  TFile file(rf.c_str(), "read");
	TTree *events = (TTree*)file.Get("sim_out");
	events->SetBranchAddress("simEvent", &event_output);

	TFile file2(wf.c_str(), "recreate");
  TTree *tree = new TTree("output", "a tree of Reconstruction Data");
	tree->Branch("recEvent", &reconstruction);

	TFile file3(modified_eventoutput_file.c_str(), "recreate");
	TTree *modified_events = new TTree("sim_out", "a tree of the modified particle information");
	modified_events->Branch("simEvent", &modified_event_output);
  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nRECONSTRUCTOR\n";

	disp.checker = checker;
  for (i = 0; i < events->GetEntries(); i++)
  {
		cout << "Event " << i << "\n";
    if (print == true)
    {
  		disp.Main = "yes"; disp.Action = "yes"; disp.Specific = "no"; disp.General = "yes"; disp.Data = "yes"; 
  	}
  	if (i != checker) disp = disp_def;
	  events->GetEntry(i);

	  // cout << i <<"--------> problems is after this?\n";
    if (ai.last_given){
      beginning = event_output->Particles.size() - last;
      if (beginning>0){
	      for (unsigned j = 0; j < beginning; ++j)
	      	event_output->Particles.erase(event_output->Particles.begin());
      }
    }
    // cout << i <<"--------> problems is before this?\n";
	  ReconstructEvent(reconstruction, event_output, disp);
	  if ((print == true) && (reconstruction.Photons.size() == 0)) 
	  	printf("\tEvent had no reconstructions");
  	modified_event_output = event_output;
	  if (modified)
		  modified_events->Fill();
	  tree->Fill();
  }

  file3.cd();
  if (modified)
	  file3.Write();
  file3.Close();

  file2.cd();
  file2.Write();
  file2.Close();
  
  file.cd();
  file.Close();


  // modified_events->Print();
  cout << "reconstruction file: " << wf << endl;
  if (modified)

	  cout << "modified particles file: " << modified_eventoutput_file << endl;

  return 0;
}