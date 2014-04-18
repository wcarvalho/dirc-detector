//===================================================
//                Printer
//
//====================================================

#include "../headers/printer.h"

int main()
{

	int i, j;

	Printer printer;
	Displayer display;

  GeneratorOut* event_output = 0;
	Reconstruction* reconstruction = 0;

	TFile f1("../../dirc_events.root", "read");
	TFile f2("../../ReconstructionData.root", "read");

	TTree *events = (TTree*)f1.Get("events");
	TTree *output = (TTree*)f2.Get("output");

	events -> SetBranchAddress("event_output", &event_output);
	output -> SetBranchAddress("reconstruction", &reconstruction);


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
		  printer.Hist_Title = to_string(event_output->Particles.at(j).Theta);
		  printer.Name = "../../Graphs/Event_";
		  string WhichEvent = fixedLength(i, 2);
		  printer.Name.append(WhichEvent);
			printer.Name.append("_Particle_");
			string s0 = to_string(j+1);			
			printer.Name.append(s0);
			
			Print_EventTH2D(reconstruction, printer, display);
			Print_EventTH1D(reconstruction, printer, display);
			
			printer.Name.clear();
			
	  } 
		printer.Name = "../../Graphs/Event_";
		string WhichEvent = to_string(i);
		printer.Name.append(WhichEvent);
		printer.Name.append(".png");
		// Print_EventGraph();
	}



}