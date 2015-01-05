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

  double pi = TMath::Pi();

  int last = 0;

  int xbins = 1000;
	int ybins = 1000;

	double smear = .01;

 	int ExpectedPhotonCase = ai.expected_photons_case_arg;

 	// i/o setup
	string graphprefix = "";
  string rf_default = ai.input_arg;
  string wf_default = "reconstruction.root"; 
	string rf = rf_default;
  string wf = wf_default;

  wul::FileProperties readf_prop(rf);
  string directory = readf_prop.directory;

  // parse ai arguments
	bool print = ai.verbose_given;
  bool quiet = ai.quiet_given; if (quiet) print = !quiet;
	bool make = ai.make_given;
  if (ai.last_given) last = ai.last_arg;
  if (ai.write_file_given) wf = ai.write_file_arg;
	if (ai.Directory_given) directory = ai.Directory_arg; readf_prop.appendFileToDirectory(directory, wf);
	if (ai.Smear_given) smear = ai.Smear_arg;
	if (ai.graph_prefix_given) graphprefix = ai.graph_prefix_arg;

	
	// Function Pointer(s)
	double (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&);


	// determine which function will be used to determine the expected number of photons
	if (print) cout << "ExpectedPhotonCase = ";
	switch(ExpectedPhotonCase) {
		case 1: // look-up table
			if (print) cout << "LookUpTable\n";
			ExpectedNumberofPhotons = &LookUpTableWrapper;
		break;
		case 2: // riemansum
			if (print) cout << "RiemannSum\n";
			ExpectedNumberofPhotons = &RiemannSum;
		break;
	}

	// Classes used for analysis
  Reconstruction reconstruction;							// used to reconstruct original photon trajectories
	ReconstructionData data;
	reconstruction.Track.push_back(data);				
	Analysis A;																	// class used to create histograms												
	TrackRecons Tracks; Tracks.Recon.clear();		// stores information on particle identity guesses

  // pointers to data from ROOT File
  GeneratorOut *event_output = 0;
  Detector* d = 0;

  TFile file(rf.c_str(), "read");
	TTree *events = (TTree*)file.Get("sim_out");
	events->SetBranchAddress("simEvent", &event_output);
	events->SetBranchAddress("detector", &d);
	

	TFile file2(wf.c_str(), "recreate");
  TTree tree_np("identifications", "information on what particles tracks were reconstructed into, and with what probability");
	TTree* tree = &tree_np;
	tree->Branch("guesses", &Tracks);
	tree->Branch("detector", &d);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------

  for (unsigned int ev = 0; ev < events->GetEntries(); ++ev)
  {
		if (!quiet) cout << "Event " << ev << "\n";
		events->GetEntry(ev);

		// Declarations
		TrackRecon guess;
		guess.clear();
		vector<ParticleOut> &pars = event_output->Particles;
		// ---------------

    removeFirstParticles(ai.last_given, event_output, last); 	// remove all particles except for last particles determined by option 'l'
	  ReconstructEvent(reconstruction, event_output, print);
	  
	  if (print && !(reconstruction.Photons.size())){
	  	printf("\tEvent %i had no reconstructions\n", ev);
	  	continue;
	  }
  	
	  for (unsigned int par = 0; par < pars.size(); par++){
	  	if (!quiet) cout << "\tParticle " << par << endl;
  		
  		vector<PhotonOut> &phos = reconstruction.Photons.at(par);
			if ( !(phos.size()) ) continue;
			CreateHistogram_1D2D(ev, par, A, phos, xbins, ybins);

			TH1D *h1_p = &A.Hists1D.back();
			guess.Hist = *h1_p;
			ParticleOut &P = pars.at(par);
			
			if (print) printf("\tpar = %i: eta = %f, pt = %f\n", par+1, P.Eta, P.pt);

			CalculateParticleFits(ExpectedNumberofPhotons, P, h1_p, guess, .1, smear, print);				// for one particle, 1 fit is calculated for every possible mass (5 masses means 5 fits for 1 particle)
			Tracks.Recon.push_back(guess);
	  }


	  tree->Fill();
		Tracks.Recon.clear();
  }

  file2.cd();
  file2.Write();
  file2.Close();
  
  file.cd();
  file.Close();


  if (!quiet) cout << "reconstruction file: " << wf << endl;

  return 0;
}