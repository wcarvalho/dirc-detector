#include "dirc_objects.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// #include <sys/time.h>

#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
#include <tclap/CmdLine.h>

using namespace std;
int main(int argc, char* argv[]){

string photonsin;
string cheatin;

string photonsout;
string cheatout;

double smear;

bool submittedCheatFile;
bool print;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> photoninFileArg("p","photon-in-file","file with results from simulation data (photon data)",true,"photons.root","string", cmd);
	TCLAP::ValueArg<std::string> photonoutFileArg("P","photon-out-file","file with results from simulation data (photon data)",true,"photons.root","string", cmd);

	TCLAP::ValueArg<std::string> cheatinFileArg("c","cheat-in-file","file with cheat data from simulation",false,"cheat.root","string", cmd);
	TCLAP::ValueArg<std::string> cheatoutFileArg("C","cheat-out-file","file with cheat data from simulation",false,"cheat.root","string", cmd);

	TCLAP::ValueArg<double> smearArg("s","smear","the value with each the angle will be smeared",false,0.01,"string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv);
	photonsin = photoninFileArg.getValue();
	photonsout = photonoutFileArg.getValue();

	submittedCheatFile = cheatinFileArg.isSet();
	if (submittedCheatFile){
		cheatin = cheatinFileArg.getValue();
		cheatout = cheatoutFileArg.getValue();
	}
	smear = smearArg.getValue();
	print = verboseArg.getValue();
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	TRandom3 r;
	GeneratorOut *event_output = 0;
	PhotonEvent *photon_event = 0;
	// need Detector to preserve structure of cheat and sim. both carry the detector information
	Detector *d = 0;

	TFile fin1(photonsin.c_str(), "read");
	TTree* inputTree1 = (TTree*)fin1.Get("sim_out");
	inputTree1->SetBranchAddress("simEvent", &event_output);
	inputTree1->SetBranchAddress("detector", &d);

	TFile fout1(photonsout.c_str(), "recreate");
	TTree outputTree1("sim_out", "smeared simulation output");
	outputTree1.Branch("simEvent", &event_output);
	outputTree1.Branch("detector", &d);

	TFile* fin2 = 0;
	TTree* inputTree2 = 0;
	TFile* fout2 = 0;
	TTree* outputTree2 = 0;

	if (submittedCheatFile){
		fin2 = TFile::Open(cheatin.c_str(), "read");
	 	inputTree2 = (TTree*)fin2->Get("cheat_info");
		inputTree2->SetBranchAddress("Photon Event", &photon_event);
		inputTree2->SetBranchAddress("detector", &d);

		fout2 = TFile::Open(cheatout.c_str(), "recreate");
		outputTree2 = new TTree("cheat_info", "smeared simulation output");
		outputTree2->Branch("Photon Event", &photon_event);
		outputTree2->Branch("detector", &d);
	}

	vector<Photon>* Photons = 0;
	for (unsigned int ev = 0; ev < inputTree1->GetEntries(); ++ev){
		inputTree1->GetEntry(ev);
		outputTree1.GetEntry(ev);
		if (submittedCheatFile){
			inputTree2->GetEntry(ev);
			outputTree2->GetEntry(ev);
		}
		// for each event, iterate over the photons
		vector<PhotonOut> &PhotonOuts = event_output->Photons;
		if (submittedCheatFile) Photons = &photon_event->Photons;
		for (unsigned int pho = 0; pho < PhotonOuts.size(); ++pho){
			double theta_smear = r.Gaus(0, smear);
			double phi_smear = r.Gaus(0, smear);
			if (submittedCheatFile){
				Photons->at(pho).Theta += theta_smear;
				Photons->at(pho).Phi += phi_smear;
			}
			PhotonOuts.at(pho).Theta += theta_smear;
			PhotonOuts.at(pho).Phi += phi_smear;
		}
		outputTree1.Fill();
		if (submittedCheatFile) outputTree2->Fill();
	}

	fin1.cd();
	fin1.Close();
	fout1.cd();
	fout1.Write();
	fout1.Close();

	if (submittedCheatFile){
		fin2->cd();
		fin2->Close();
		fout2->cd();
		fout2->Write();
		fout2->Close();
	}

	return 0;

	// cout << "file: " << outputfile << endl;
}
