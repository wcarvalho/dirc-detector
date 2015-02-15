/**
 * 					Reconstructor
 */

#include "../headers/reconstructor.h"
#include <cstdlib>
#include "cmdline.h"
#include "boost/bind.hpp"
#include "boost/function.hpp"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }

    double pi = TMath::Pi();

    int last     = 0;
    int xbins    = 1000;
    int ybins    = 1000;
    double smear = .01;

 		int ExpectedPhotonCase = ai.expected_photons_case_arg;


    // measuring time
    timeval t1, t2;
    double time1 = 0;
    double time2 = 0;
    double calculateFits_time=0.;
    //
    // i/o setup
    string graphprefix = "";
    string rf_default = ai.input_arg;
    string wf_default = "reconstruction.root";
    string rf = rf_default;
    string wf = wf_default;

    wul::FileProperties readf_prop(rf);
    string directory = "";




    // parse ai arguments
	bool print = ai.verbose_given;
	bool quiet = ai.quiet_given; if (quiet) print = !quiet;
	bool make  = ai.make_given;
    if (ai.last_given) last = ai.last_arg;
    if (ai.write_file_given) wf = ai.write_file_arg;

    if(ai.Directory_given) {
        string temp_dir = ai.Directory_arg;
        if (temp_dir.size() != 0)	directory = ai.Directory_arg;
    	else directory = readf_prop.directory;
	}
	readf_prop.appendFileToDirectory(directory, wf);


	if (ai.Smear_given) smear = ai.Smear_arg;
	if (ai.graph_prefix_given) graphprefix = ai.graph_prefix_arg;


	// double lengths_low[5] = {0, 0, 0, 0, .7};
	// double lengths_hi[5] = {490, 3.5, pi/2, 2*pi, 1};
	// int nbins[5] = {20, 5, 10, 10, 5};
	// string lookupfile = "LookUpTable";






	// Function Pointer(s)
	std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&);

  // boost::function<*double(double const&, double const&, double const&, double const&, double const&)> ExpectedNumberofPhotons;

	// determine which function will be used to determine the expected number of photons
	if (print) cout << "ExpectedPhotonCase = ";
	switch(ExpectedPhotonCase) {
		case 1: // look-up table
			if (!quiet) cout << "LookUpTable\n";

			// ExpectedNumberofPhotons = boost::bind( &LookUpTableWrapper, _1, _2, _3, _4, _5, lengths_low, lengths_hi, nbins, lookupfile);
			ExpectedNumberofPhotons = &LookUpTableWrapper;
		break;
		case 2: // riemansum
			if (!quiet) cout << "RiemannSum\n";
			ExpectedNumberofPhotons = &RiemannSum;
		break;
	}

	// Classes used for analysis
  Reconstruction reconstruction;							// used to reconstruct original photon trajectories
	ReconstructionData data;
	reconstruction.Track.push_back(data);
	Analysis A;																	// class used to create histograms
	TrackRecons Tracks;		// stores information on particle identity guesses

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

		ParticleOut &P = pars.at(par);
		if (print) printf("\tpar = %i: eta = %f, pt = %f\n", par+1, P.Eta, P.pt);

		gettimeofday(&t1, NULL);
		CalculateParticleFits(ExpectedNumberofPhotons, P, A, .1, smear, print);				// for one particle, 1 fit is calculated for every possible mass (5 masses means 5 fits for 1 particle)
		gettimeofday(&t2, NULL);
		time1 = (double)(t1.tv_sec) + (double)(t1.tv_usec)/1.0e6;
		time2 = (double)(t2.tv_sec) + (double)(t2.tv_usec)/1.0e6;
		calculateFits_time += (time2-time1);

		if (print) cout << "TrackRecons has " << Tracks.Recon.size() << " Tracks\n";
	  }

	  Tracks.Recon.push_back(A.Recon.back());
	  tree->Fill();
		A.Recon.clear();
		Tracks.Recon.clear();
  }

  file2.cd();
  file2.Write();
  file2.Close();

  file.cd();
  file.Close();

  if (!quiet) cout << "Time to calculate fits = " << calculateFits_time << endl;
  if (!quiet) cout << "reconstruction file: " << wf << endl;

  return 0;
}