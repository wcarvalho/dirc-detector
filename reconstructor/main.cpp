/**
 * 					Reconstructor
 */

#include "reconstructor.h"
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

	unsigned band_search_case = ai.band_search_case_arg;

	// double lengths_low[5] = {0, 0, 0, 0, .7};
	// double lengths_hi[5] = {490, 3.5, pi/2, 2*pi, 1};
	// int nbins[5] = {20, 5, 10, 10, 5};
	// string lookupfile = "LookUpTable";

	// determine which function will be used to determine the expected number of photons
	if (print) cout << "ExpectedPhotonCase = ";
	std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&);
	switch(ExpectedPhotonCase) {
		case 1: // look-up table
			if (!quiet) cout << "LookUpTable\n";
			ExpectedNumberofPhotons = &LookUpTableWrapper; break;
		case 2: // riemansum
			if (!quiet) cout << "RiemannSum\n";
			ExpectedNumberofPhotons = &RiemannSum; break;
	}

	// Classes used for analysis
  Reconstruction reconstruction;							// used to reconstruct original photon trajectories
	// ReconstructionData data;
	// reconstruction.Track.push_back(data);
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
          TTree tree_np("identifications", "information on what particles tracks were reconstructed into and with what probability");
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

		vector<ParticleOut> &pars = event_output->Particles;
    if (ai.last_given) removeFirstParticles(event_output, last, print); 	// remove all particles except for last particles determined by option 'l'
		int npar = pars.size();
		if (npar == 0) continue;
	  ReconstructEvent(reconstruction, event_output, print);
	  if (reconstruction.Photons.size() == 0)	continue;

		for (unsigned i = 0; i < reconstruction.Photons.at(0).size(); ++i)
			A.index.push_back(-10);				// for each photon set photon to a value that won't be used (e.g. -10)

		auto index_copy = A.index;

	  static unordered_map <int, int> photon_overlap;
	  photon_overlap.clear();
	  for (unsigned i = 0; i < pars.size(); ++i)
	  	photon_overlap[i] = 0;



		static double momentum_indexing_threshold = ai.momentum_indexing_threshold_arg;
	  for (unsigned int par = 0; par < pars.size(); par++){
			vector<PhotonOut> &phos = reconstruction.Photons.at(par);

			if ( !(phos.size()) ) continue;
			CreateHistogram_1D2D(ev, par, A, phos, xbins, ybins);
			A.AddTrackRecon();
			TrackRecon &guess  = A.Recon.back();
			guess.Hist2D       = A.Hists2D.back();


			ParticleOut &P = pars.at(par);

			if (passed_index_photons_condition(P, momentum_indexing_threshold)){
				if (print) cout << "Indexing particle " << par << endl;
				IndexPhotons(P, par, phos, A, smear, band_search_case, photon_overlap, print);
			}
			else
				if (print) cout << "\tNot indexing particle " << par << endl;

	  }


	  for (unsigned int par = 0; par < pars.size(); par++){
			auto& phos = reconstruction.Photons.at(par);
			auto& P = pars.at(par);

			TH1D* reduced_histogram_theta_projection = ReducedHistogram(phos, A, par);
			

			if (print) cout << "Fitting particle " << par << endl;
			

			gettimeofday(&t1, NULL);
			CalculateParticleFits(ExpectedNumberofPhotons, *reduced_histogram_theta_projection, P, phos, A, par, smear, photon_overlap[par], print);				// for one particle, 1 fit is calculated for every possible mass (5 masses means 5 fits for 1 particle)
			gettimeofday(&t2, NULL);


			time1 = (double)(t1.tv_sec) + (double)(t1.tv_usec)/1.0e6;
			time2 = (double)(t2.tv_sec) + (double)(t2.tv_usec)/1.0e6;
			calculateFits_time += (time2-time1);
			delete reduced_histogram_theta_projection;
	  }

	  Tracks.Recon = std::move(A.Recon);
	  Tracks.index = std::move(A.index);
	  tree->Fill();
		A.Hists1D.clear(); A.Hists1D.shrink_to_fit(); // freeing (a LOT of) memory
		A.Hists2D.clear(); A.Hists2D.shrink_to_fit(); // freeing (a LOT of) memory
		A.Recon.clear();
	  A.index.clear();
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