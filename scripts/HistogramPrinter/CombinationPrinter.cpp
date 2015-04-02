#include <string>
#include <sstream>
#include <unordered_map>

#include "TFile.h"
#include "TTree.h"

#include "passConditions.h"
#include "reconstructor.h"
#include "dirc_objects.h"
#include "createScatterPlot.h"
#include "CombinationPrinter.h"
#include <tclap/CmdLine.h>

using namespace std;

int main(int argc, char const *argv[])
{
	string photon_file;
	string particle_file;
	string reconstruction_file;
	string directory;
	string basename;

	string particle_search;
	string particle_compare;

	int max_count;
	double threshold;

	vector<int> matchcondition_cases = {1, 4};
	vector<double> momentum_range {1.75, 2.25};

	bool print = false;

	TCLAP::CmdLine cmd("Options for printing", ' ', "0.1");
	try{
		TCLAP::ValueArg<string> directoryArg("D", "directory", "directory where plots will be stored", false, "", "string", cmd);
		TCLAP::ValueArg<string> basenameArg("b", "basename", "base used for filenames (e.g. 'base'_particletype.root, 'base'_particlecomparison.root)", false, "", "string", cmd);
		TCLAP::ValueArg<string> particle_fileArg("p", "particle-file", "particle information", false, "particle_file.root", "string", cmd);
		TCLAP::ValueArg<string> photon_fileArg("P", "photon-file", "photon information", false, "photon_file.root", "string", cmd);
		TCLAP::ValueArg<string> reconstruction_fileArg("r", "reconstruction-file", "reconstruction information", false, "reconstruction_file.root", "string", cmd);
			TCLAP::MultiArg<double> momentum_rangeArg("m","momentum-range","momentum range for which histograms are printed", false, "double", cmd);

		TCLAP::ValueArg<string> particle_searchArg("s", "particle-search", "particle used for search", false, "pion", "string", cmd);
		TCLAP::ValueArg<string> particle_compareArg("c", "particle-compare", "particle used for comparison", false, "electron", "string", cmd);

		TCLAP::ValueArg<double> thresholdArg("t","threshold","threshold used for match-condition", false, 1,"double", cmd);
		TCLAP::MultiArg<int> matchconditionArg("M","match-condition","sets the method by which matches will be determined."
			"\n\t\tcase 1: within_angle_resolution"
			"\n\t\tcase 2: inside_box"
			"\n\t\tcase 3: inside_circle"
			"\n\t\tcase 4: xyplane",false, "double", cmd);

		TCLAP::ValueArg<int> max_countArg("C","max-count","number of each type of graph",false, 100,"int", cmd);

		cmd.parse( argc, argv);
		directory = directoryArg.getValue();
		basename = basenameArg.getValue();
		photon_file = photon_fileArg.getValue(); 
		particle_file = particle_fileArg.getValue(); 
		reconstruction_file = reconstruction_fileArg.getValue(); 	
		particle_search = particle_searchArg.getValue(); 	
		particle_compare = particle_compareArg.getValue(); 	
	
		threshold = thresholdArg.getValue();
		max_count = max_countArg.getValue();

		if (momentum_rangeArg.isSet())
			momentum_range = momentum_rangeArg.getValue();

		if (matchconditionArg.isSet())
			matchcondition_cases = matchconditionArg.getValue();

	}
	catch( TCLAP::ArgException& e ){ 
		cout << "ERROR: " << e.error() << " " << e.argId() << endl;
	}

	func_map functions;	
	functions[1] = &within_angle_resolution;
	functions[2] = &inside_box;
	functions[3] = &inside_circle;
	functions[4] = &xyplane;


	TFile photon_Tfile(photon_file.c_str(), "read"); checkValid(photon_Tfile);
	TTree* photon_tree = (TTree*)photon_Tfile.Get("sim_out");
	GeneratorOut* photon_event = 0;
	photon_tree->SetBranchAddress("simEvent", &photon_event);


	TFile particle_Tfile(particle_file.c_str(), "read"); checkValid(particle_Tfile);
	TTree* particle_tree = (TTree*)particle_Tfile.Get("cheat_info");
	ParticleEvent *originals = 0;
	particle_tree->SetBranchAddress("Particle Event", &originals);

	TFile reconstruction_Tfile(reconstruction_file.c_str(), "read"); checkValid(reconstruction_Tfile);
	TTree* reconstruction_tree = (TTree*)reconstruction_Tfile.Get("identifications");
	TrackRecons *reconstructions = 0;
	reconstruction_tree->SetBranchAddress("guesses", &reconstructions);
	

	auto &pars   = originals->Particles;
  auto &recons = reconstructions->Recon;
	vector<int>& index = reconstructions->index;

	stringstream ss;
	auto filename = [&ss, &directory, &basename, &particle_search, &momentum_range](string const input){
		ss.str(""); ss << directory << "/"<< basename << "_"<< particle_search << input << "_p_" << momentum_range.at(0) << "_" << momentum_range.at(1) << ".root";
		return ss.str();
	};

	TFile particleEvents(filename("").c_str(), "recreate");
	unsigned particleEvents_count = 0;
	
	ss.str("");
	ss << "_as_" << particle_compare;

	TFile misIdentificationEvents(filename(ss.str()).c_str(), "recreate");
	unsigned misIdentificationEvents_count = 0;
	
	unsigned nentries = reconstruction_tree->GetEntries();

	for (unsigned entry = 0; entry < nentries; ++entry){
		photon_tree->GetEntry(entry);
		particle_tree->GetEntry(entry);
		reconstruction_tree->GetEntry(entry);		

		static Reconstruction photon_reconstruction;
		ReconstructEvent(photon_reconstruction, photon_event, print);
		
		vector<ParticleOut> par_outs(pars.begin(), pars.end());

    if ((recons.size() == 0) || (pars.size() == 0)) continue;
		for (unsigned i = 0; i < pars.size(); ++i){
			auto& par = pars.at(i);
			auto& phos = photon_reconstruction.Photons.at(i);
			auto& recon = recons.at(i);
			double momentum = par.CalculateMomentum();
			if (!withinrange(momentum_range, momentum)) continue;

			int particle_search_index = getReconIndex(recon, particle_search);
		
			if ((par.name == particle_search) && (particleEvents_count < max_count))
				AddCombination(particleEvents, par_outs, phos, index, i, recon, particle_search_index, momentum, particle_search, particleEvents_count);
			
			bool passed = passConditions(matchcondition_cases, functions, par, recon, particle_search_index, threshold);
			if (passed && (par.name == particle_search) && (misIdentificationEvents_count < max_count))
				AddCombination(misIdentificationEvents, par_outs, phos, index, i, recon, particle_search_index, momentum, particle_search, misIdentificationEvents_count);

			if ((misIdentificationEvents_count == max_count) && (particleEvents_count == max_count))
				return 0;
		}


	}





	return 0;
}
