#include "../folders.h"
#include "pixelate_photons.h"

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

	if ( ! (options[SIMDATA] && options[OUTPUT] && options[THRESHDOUBLE]) ){
		std::cout << "Must provide simdata (via -S option), output (via -o option), and resolution (via -T option)!!\n";
		option::printUsage(std::cout, usage);
		return 0;
	}
	std::string inputfile = options[SIMDATA].arg;
	std::string outputfile = options[OUTPUT].arg;
	double resolution = atof(options[THRESHDOUBLE].arg);
	
	GeneratorOut *event_output = 0;

	// read in inputfile, outputfile
	// create Tfile for input and output files
	TFile f1(inputfile.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("sim_out");
	t1->SetBranchAddress("simEvent", &event_output);

	TFile f2(outputfile.c_str(), "recreate");
	TTree outputTree("sim_out", "pixelated photons");
	TTree *t2 = &outputTree;
	t2->Branch("simEvent", &event_output);


	double pi = 3.14159265358979312;

	double phibounds[2] = {-pi, pi};
	double thetabounds[2] = {0, pi};

	int phibins = nbins(phibounds, resolution);
	int thetabins = nbins(thetabounds, resolution);
	std::cout << "phibins = " << phibins << std::endl;
	std::cout << "thetabins = " << thetabins << std::endl;
	// define bounds of 2D array
	// phi the x-axis? 0 to pi
	// theta is the y-axis? -pi to pi
	// each have the resolution of the smearing?
	for (unsigned int ev = 0; ev < t1->GetEntries(); ++ev){
		t1->GetEntry(ev);
		t2->GetEntry(ev);


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
}
