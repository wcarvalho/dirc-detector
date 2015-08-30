/**
 * 					Reconstructor
 */

#include "reconstructor.h"
#include "dirc_io.h"
#include <cstdlib>
#include "cmdline.h"

#define pi 3.14159265358979312

int main(int argc, char** argv)
{
	gengetopt_args_info ai;
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }

	// command line options with default arguments
	//----------------------------------------------------------------------------
	bool print = ai.verbose_given;
	bool quiet = ai.quiet_given; if (quiet) print = !quiet;
  double angle_smear = ai.as_arg;
  double time_smear = ai.ts_arg;
  double time_error = ai.terr_arg;
	unsigned band_search_case = ai.band_search_case_arg;
	double band_search_width = ai.band_search_width_arg;
  string rf = ai.input_arg;								// read file / input
  string wf = ai.output_arg;							// write file / output

	// command line options without default arguments
  //----------------------------------------------------------------------------
  // set the directory for the output file
	  dirc::FileProperties rf_properties(rf);
	  string directory = "";
	  if(ai.Directory_given) {
	    string temp_dir = ai.Directory_arg;
	    if ( !temp_dir.empty() ) directory = ai.Directory_arg;
	  	else directory = rf_properties.directory;
		}
		rf_properties.appendFileToDirectory(directory, wf);
	//----------------------------------------------------------------------------
	// set the cases that will be used to index the photons (inc = index cases)
		vector<int> band_cases;
		if (ai.inc_given){
			if (!quiet) cout << "cases used:\t";
			for (unsigned i = 0; i < ai.inc_given; ++i){
				auto& C = ai.inc_arg[i];
				band_cases.push_back(C);
				if (!quiet) cout << C << "\t";
			}
			if (!quiet) cout << endl;
		}
		else
			band_cases = {1, 2};
		if (band_cases.at(0) == 4) band_cases = {1, 2};
	//----------------------------------------------------------------------------
	// determine which function will be used to determine the expected number of photons
		if (print) cout << "ExpectedPhotonCase = ";
		std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&);
		switch(ai.expected_photons_case_arg) {
			case 1: // look-up table
				if (!quiet) cout << "LookUpTable\n";
				ExpectedNumberofPhotons = &LookUpTableWrapper; break;
			case 2: // riemansum
				if (!quiet) cout << "RiemannSum\n";
				ExpectedNumberofPhotons = &RiemannSum; break;
		}
	//----------------------------------------------------------------------------

	// root files i/o
  GeneratorOut *event_output = 0;				// monte-carlo data
  Detector* d = 0;											// detector information
	TrackRecons Tracks;										// stores particle reconstruction data

	TFile file;
	TFile* file_p = &file;
	TTree *events;
	readInGeneratorData(file_p, rf, events, event_output, d);

	TFile file2(wf.c_str(), "recreate");
  TTree tree_np("identifications", "information on what particles tracks were reconstructed into and with what probability");
	TTree* tree = &tree_np;
	tree->Branch("guesses", &Tracks);
	tree->Branch("detector", &d);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------

  for (unsigned ev = 0; ev < events->GetEntries(); ++ev){
		events->GetEntry(ev);
		static bool event_is_in_focus;
		if (ai.fe_given) event_is_in_focus = (ev == ai.fe_arg);

		vector<ParticleOut> &particles = event_output->Particles;


		if (!quiet) cout << "Event " << ev << ", with " << particles.size() << " particles and " << event_output->Photons.size() << " photons\n";

    // remove all particles except for the last particles determined by option 'l'
    if (ai.last_given) removeFirstParticles(event_output, ai.last_arg, print);


		auto reconstructed_photons = reconstruct_photons(event_output->Photons); // find all reflections photons might have udnergone

    // initialize variables that will be used
		static Photon_Sets photons_in_different_frames;		// 1 set of photons per particle
		vector<int>& index = Tracks.index; 				// used to color photons
		static unsigned nphotons;
    static unsigned nparticles;

		// reset variables
    nparticles = particles.size();
		nphotons = reconstructed_photons.size();
		reset_photons_in_different_frames(photons_in_different_frames, nparticles);
		reset_Tracks(Tracks, nparticles, nphotons);

		if ( particles.empty() ){
			tree->Fill(); continue;
		}

		static auto const& particle_types = particles.at(0).deftypes; // get particle types

		static unordered_map <int, int> photons_per_particle;
		reset_photons_per_particle(photons_per_particle, nparticles);

		static unordered_map <int, vec_pair> expectedPhotonMap;
		getExpectedPhotonMap(particles, expectedPhotonMap, ExpectedNumberofPhotons);

	  /////////// rotate photons and index them for every particle
		static bool will_focus_particle = ai.fp_given;
		static int particle_to_focus {0};
		static bool focus_particle;
		if (will_focus_particle){
			particle_to_focus = ai.fp_arg;
			if (print) cout << "particle_to_focus = " << particle_to_focus << endl;
		}
		for (unsigned i = 0; i < nparticles; ++i){
			focus_particle = (i == particle_to_focus);
			auto& photons_in_frame = photons_in_different_frames.at(i);
			auto& particle = particles.at(i);
			auto& histogram_photons_in_frame = Tracks.Recon.at(i).Hist2D;

			photons_in_frame = std::move(rotate_photons_into_particle_frame(particle.Theta, particle.Phi, reconstructed_photons));

			check_reconstructed_photons(photons_in_frame);
			histogram_photons_in_frame = histogram_photon_angles(ev, i, photons_in_frame);

			if (band_cases.at(0) == 0) continue;
			index_photons(particle, i, photons_in_frame, index, histogram_photons_in_frame, angle_smear, time_smear, time_error, band_cases, band_search_case, band_search_width, photons_per_particle, expectedPhotonMap[i], *d, print);
		}

		////////// Create 1D Histograms and Fit them
		for (unsigned i = 0; i < nparticles; ++i){
			focus_particle = (i == particle_to_focus);
			auto& photons_in_frame = photons_in_different_frames.at(i);
			auto& particle = particles.at(i);
			auto& current_recon = Tracks.Recon.at(i);
			auto& Hist2D = current_recon.Hist2D;

			TH1D* reduced_histogram_theta_projection = 0;
			if (band_cases.at(0) != 0)
				reduced_histogram_theta_projection = ReducedHistogram(photons_in_frame, Hist2D, index, i);
			else
				reduced_histogram_theta_projection = Hist2D.ProjectionY();

			if (!quiet)
				cout << "\tFitting particle " << i << " with " << photons_per_particle[i] << " photon angles (" << (int)photons_per_particle[i]/4 << ") at x = " << particle.X << endl;

			if ( (photons_per_particle[i] != 0 ) || (band_cases.at(0) == 0) )
				CalculateParticleFits(*reduced_histogram_theta_projection, particle, current_recon, expectedPhotonMap[i], i, angle_smear, print);
			if (band_cases.at(0) != 0) delete reduced_histogram_theta_projection;
		}

		tree->Fill();
  }

  file2.cd();
  file2.Write();
  file2.Close();

  file.cd();
  file.Close();

  if (!quiet) cout << " file: " << wf << endl;

  return 0;
}