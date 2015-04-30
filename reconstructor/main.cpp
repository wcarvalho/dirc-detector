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
	double band_search_width = ai.band_search_width_arg;

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
    // remove all particles except for last particles determined by option 'l'
    if (ai.last_given) removeFirstParticles(event_output, last, print);

    unsigned npars = pars.size();
		if (print) cout << "\t" << npars << " particles\n";
		if ( pars.empty() ){ tree->Fill(); continue; }


		auto const& particle_types = pars.at(0).deftypes; // get particle types
		auto reconstructed_photons = reconstruct_photons(event_output->Photons); // find all reflections photons might have udnergone
		unsigned nphotons = reconstructed_photons.size();
		if ( reconstructed_photons.empty() ){ tree->Fill(); continue; }

		static Photon_Sets photons_in_different_frames;		// 1 set of photons per particle
		reset_photons_in_different_frames(photons_in_different_frames, npars);

		reset_Tracks(Tracks, npars, nphotons);
		vector<int>& index = Tracks.index; 				// used to color photons

		// transform time_traveled into length_traveled
		for (auto& photon: reconstructed_photons){
			photon.Time_Traveled *= 300*TMath::Cos(photon.Phi)*TMath::Sin(photon.Theta);
		}

		static unordered_map <int, int> photons_per_particle;
		reset_photons_per_particle(photons_per_particle, npars);

		static unordered_map <int, vec_pair> expectedPhotonMap;
		getExpectedPhotonMap(pars, expectedPhotonMap, ExpectedNumberofPhotons);

	  /////////// rotate photons and index them for every particle
		for (unsigned i = 0; i < npars; ++i){
			auto& photons_in_frame = photons_in_different_frames.at(i);
			auto& particle = pars.at(i);
			auto& histogram_photons_in_frame = Tracks.Recon.at(i).Hist2D;

			photons_in_frame = std::move(rotate_photons_into_particle_frame(particle.Theta, particle.Phi, reconstructed_photons));
			histogram_photons_in_frame = histogram_photon_angles(ev, i, photons_in_frame);

			index_photons(particle, i, photons_in_frame, index, histogram_photons_in_frame, smear, band_search_case, band_search_width, photons_per_particle, expectedPhotonMap[i], print);
		}

		////////// Create 1D Histograms and Fit them
		for (unsigned i = 0; i < npars; ++i){
			auto& photons_in_frame = photons_in_different_frames.at(i);
			auto& particle = pars.at(i);
			auto& current_recon = Tracks.Recon.at(i);

			TH1D* reduced_histogram_theta_projection = ReducedHistogram(photons_in_frame, current_recon.Hist2D, index, i);
			if (print) cout << "\t\tFitting particle " << i << " with " << photons_per_particle[i] << " expected photons\n";

			if (photons_per_particle[i] != 0 )
				CalculateParticleFits(*reduced_histogram_theta_projection, particle, current_recon, expectedPhotonMap[i], i, smear, print);

			delete reduced_histogram_theta_projection;
		}

		tree->Fill();

  }

  file2.cd();
  file2.Write();
  file2.Close();

  file.cd();
  file.Close();

  if (!quiet) cout << "reconstruction file: " << wf << endl;

  return 0;
}