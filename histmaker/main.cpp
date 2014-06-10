//===================================================
//                Histogram Maker
//
//====================================================

#include "../headers/histmaker.h"
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

	Displayer display;

  GeneratorOut* event_output = 0;
	Reconstruction* reconstruction = 0;
	Analysis *A = 0;

	int xbins = 1000;
	int ybins = 1000;

	string readf1 = "../../root_files/simulator.root";
	string readf2 = "../../root_files/reconstructor.root";
	string writef = "../../root_files/analysis.root";


	TFile f1(readf1.c_str(), "read");
	TFile f2(readf2.c_str(), "read");
	TFile wf(writef.c_str(), "recreate");

	TTree *events = (TTree*)f1.Get("sim_out");
  TTree *output = (TTree*)f2.Get("output");
	events -> SetBranchAddress("simEvent", &event_output);
	output -> SetBranchAddress("recEvent", &reconstruction);

  TTree* THists = new TTree("THists", "Histograms and other information for events");
	THists -> Branch("EventHists", &A);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
	cout << "\nHistogram Maker\n";
	double pi = TMath::Pi();
	for (unsigned int ev = 0; ev < events->GetEntries(); ev++)
	{
	  Printer *printer = new Printer();
		events->GetEntry(ev);
	  output->GetEntry(ev);

	  for (unsigned int par = 0; par < event_output->Particles.size(); par++)
	  {
  		vector<PhotonOut> &phos = reconstruction->Photons.at(par);
	  	vector< vector<double> > data; data.clear();
	  	vector<double> data_sub; data_sub.clear();

	  	for (unsigned int i = 0; i < phos.size(); i++)
	  	{
	  		data.push_back(data_sub);
	  			data.back().push_back(phos.at(i).Phi);
	  			data.back().push_back(phos.at(i).Theta);
	  	}
		  stringstream histtitle;
		  histtitle << "Eta =" << event_output->Particles.at(par).Eta << ", pt = " << event_output->Particles.at(par).pt;
 			
 			stringstream histname;
 			histname << setfill('0') <<setw(3) << ev;
		  histname << "_Particle_" << par+1;

		  stringstream filename;
		  filename << "../../Graphs/Event_" << histname.str();
		  
		  string title = histtitle.str();
		  string TH1Name = histname.str().append("_1D");
		  string TH2Name = histname.str().append("_2D");
		  printer->filename = filename.str();

		  printer->SetData(data);
		  printer->AddTH1D(TH1Name.c_str(), title.c_str(), xbins, 0, pi, 1);
		  printer->AddTH2D(TH2Name.c_str(), title.c_str(), xbins, -pi, pi, ybins, 0, pi);

		  // printer->PrintTH1D(par);
		  // printer->PrintTH2D(par);

	  }
	  A = printer;
		THists -> Fill();
		delete printer;
	}

wf.Write();
wf.Close();

f1.cd();
f1.Close();

f2.cd();
f2.Close();
cout << "file: " << writef << endl;

return 0;
}