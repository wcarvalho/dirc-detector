//===================================================
//                Printer
//
//====================================================

#include "../headers/printer.h"
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
	  system("exec ./../../reconstructor/build/reconstructor");
	}

  system("exec rm -rf ../../Graphs/*");

	int i, j;

	Printer printer;
	Displayer display;

  GeneratorOut* event_output = 0;
	Reconstruction* reconstruction = 0;

	TFile f1("../../root_files/simulator.root", "read");
	TFile f2("../../root_files/reconstructor.root", "read");

	TTree *events = (TTree*)f1.Get("sim_out");
  TTree *output = (TTree*)f2.Get("output");

	events -> SetBranchAddress("simEvent", &event_output);
	output -> SetBranchAddress("recEvent", &reconstruction);


  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	for (i = 0; i < events->GetEntries(); i++)
	{
		events->GetEntry(i);
	  output->GetEntry(i);
	  for (j = 0; j < event_output->Particles.size(); j++)
	  {
		  printer.which = j;		  
		  stringstream Histogram_Title;
		  Histogram_Title << "Eta =" << event_output->Particles.at(j).Eta << ", pt = " << event_output->Particles.at(j).pt;

		  printer.Hist_Title = Histogram_Title.str();
		  printer.Name = "../../Graphs/Event_";
		  string WhichEvent = fixedLength(i, 2);
		  printer.Name.append(WhichEvent);
			printer.Name.append("_Particle_");
			string s0 = to_string(j+1);			
			printer.Name.append(s0);
			
			Print_EventTH2D(reconstruction, printer, display);
			Print_EventTH1D(reconstruction, printer, display);
			
			printer.Name.clear();
			printer.Hist_Title.clear();
	  } 
		printer.Name = "../../Graphs/Event_";
		string WhichEvent = to_string(i);
		printer.Name.append(WhichEvent);
		printer.Name.append(".png");
		// Print_EventGraph();
	}



}