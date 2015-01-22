#include "Smearerheader.h"
#include "../folders.h"

int main(int argc, char* argv[]){
  argc-=(argc>0); argv+=(argc>0); // skip program name argv[0] if present
  option::Stats  stats(usage, argc, argv);
  option::Option* options = new option::Option[stats.options_max];
  option::Option* buffer  = new option::Option[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  if (options[HELP] || argc == 0) {
    option::printUsage(std::cout, usage);
    return 0;
  }
  
	for (int i = 0; i < parse.nonOptionsCount(); ++i)
	  std::cout << "Non-option #" << i << ": " << parse.nonOption(i) << "\n";

	string inputfile = "";
	string outputfile = "";
	double smear = 0.;

	if(options[SIMDATA]) inputfile = options[SIMDATA].arg;
	if(options[OUTPUT]) outputfile = options[OUTPUT].arg;
	if(options[THRESHDOUBLE])smear = atof(options[THRESHDOUBLE].arg);

	TRandom3 r;
	GeneratorOut *event_output = 0;
	Detector *d = 0;

	// create Tfile for input and output files
	TFile f1(inputfile.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("sim_out");
	t1->SetBranchAddress("simEvent", &event_output);
	t1->SetBranchAddress("detector", &d);

	TFile f2(outputfile.c_str(), "recreate");
	TTree outputTree("sim_out", "smeared simulation output");
	TTree *t2 = &outputTree;
	t2->Branch("simEvent", &event_output);
	t2->Branch("detector", &d);

	// cout << "\nSMEARER: smearing " << smear << endl;
	// iterate over events
	for (unsigned int ev = 0; ev < t1->GetEntries(); ++ev){
		// cout << "Event " << ev << endl;
		t1->GetEntry(ev);
		t2->GetEntry(ev);

		// for each event, iterate over the photons
		vector<PhotonOut> &Photons = event_output->Photons;
		// cout << "\tPhotons " << Photons.size() << endl;
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

	delete options;
	delete buffer;
	return 0;

	// cout << "file: " << outputfile << endl;
}
