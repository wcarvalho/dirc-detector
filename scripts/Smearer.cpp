#include "Smearerheader.h"

int main(){

	string settingsfile = "smearsettings.txt";
	string inputfile;
	string outputfile;
	double smear;
	TRandom3 r;

	// read in input file, outputfile and smearing value
	fillparameters(settingsfile, inputfile, outputfile, smear);
	GeneratorOut *event_output = 0;

	// create Tfile for input and output files
	TFile f1(inputfile.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("sim_out");
	t1->SetBranchAddress("simEvent", &event_output);

	TFile f2(outputfile.c_str(), "recreate");
	TTree outputTree("sim_out", "smeared simulation output");
	TTree *t2 = &outputTree;
	t2->Branch("simEvent", &event_output);

	cout << "\nSMEARER: smearing " << smear << endl;
	// iterate over events
	for (unsigned int ev = 0; ev < t1->GetEntries(); ++ev){
		cout << "Event " << ev << endl;
		t1->GetEntry(ev);
		t2->GetEntry(ev);

		// for each event, iterate over the photons
		vector<PhotonOut> &Photons = event_output->Photons;
		cout << "\tPhotons " << Photons.size() << endl;
		for (unsigned int pho = 0; pho < Photons.size(); ++pho){
			double &theta = Photons.at(pho).Theta;
			double &phi = Photons.at(pho).Phi;

			// for each photon smear by a value
			theta += r.Gaus(0, smear);
			phi += r.Gaus(0, smear);	
		}
		t2->Fill();
	}

	f1.cd();
	f1.Close();

	f2.cd();
	f2.Write();
	f2.Close();
	cout << "file: " << outputfile << endl;
}
