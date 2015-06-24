#include <string>
#include <sstream>
#include <unordered_map>

#include "TFile.h"
#include "TTree.h"

#include "passConditions.h"
#include "reconstructor.h"
#include "dirc_objects.h"
#include "dirc_io.h"
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
	int plotType;
	double threshold;
	double time_min;
	double time_max;

	vector< int > event_range;
	vector<int> matchcondition_cases = {1, 5};
	vector<double> momentum_range {1.75, 2.25};

	bool event_range_set = false;
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
		TCLAP::ValueArg<double> minArg("","min","minimum value used for time in time projection", false, 0.,"double", cmd);
		TCLAP::ValueArg<double> maxArg("","max","maximum value used for time in time projection", false, 20.,"double", cmd);

		TCLAP::ValueArg<int> plotTypeArg("","pc","type of plot to be printed"
			"\n\t\tcase 1: photons colored by particle index"
			"\n\t\tcase 2: photons colored by time"
			"\n\t\tcase 3: photons colored by particle index & colored by time. No fitting information"
			, false, 1,"int", cmd);
		TCLAP::MultiArg<int> matchconditionArg("M","match-condition","sets the method by which matches will be determined."
			"\n\t\tcase 1: within_angle_resolution"
			"\n\t\tcase 2: inside_box"
			"\n\t\tcase 3: inside_circle"
			"\n\t\tcase 4: xyplane"
			"\n\t\tcase 5: intensity_cut",false, "double", cmd);

		TCLAP::ValueArg<int> max_countArg("C","max-count","number of each type of graph",false, 100,"int", cmd);

		TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);
		TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);


		cmd.parse( argc, argv);
		directory = directoryArg.getValue();
		basename = basenameArg.getValue();
		photon_file = photon_fileArg.getValue();
		particle_file = particle_fileArg.getValue();
		reconstruction_file = reconstruction_fileArg.getValue();
		particle_search = particle_searchArg.getValue();
		particle_compare = particle_compareArg.getValue();

		time_min = minArg.getValue();
		time_max = maxArg.getValue();

		print = verboseArg.getValue();

		threshold = thresholdArg.getValue();
		max_count = max_countArg.getValue();
		plotType  = plotTypeArg.getValue();

		if (momentum_rangeArg.isSet())
			momentum_range = momentum_rangeArg.getValue();

		if (matchconditionArg.isSet())
			matchcondition_cases = matchconditionArg.getValue();

		event_range_set = eventRangeArg.isSet();
		if ( event_range_set ){
			event_range = eventRangeArg.getValue();
			if ( event_range.at(1) < event_range.at(0)){
				int temp = event_range.at(0);
				event_range.at(0) = event_range.at(1);
				event_range.at(1) = temp;
			}
		}

	}
	catch( TCLAP::ArgException& e ){
		cout << "ERROR: " << e.error() << " " << e.argId() << endl;
	}

	func_map functions;
	functions[1] = &within_angle_resolution;
	functions[2] = &inside_box;
	functions[3] = &inside_circle;
	functions[4] = &xyplane;
	functions[5] = &photon_count;

	TFile* photon_Tfile_p = 0;
	TTree* photon_tree = 0;
	GeneratorOut* photon_event = 0;
	Detector* d = 0;
	readInGeneratorData(photon_Tfile_p, photon_file, photon_tree, photon_event, d);


	TFile* particle_Tfile_p = 0;
	TTree* particle_tree = 0;
	ParticleEvent *originals = 0;
	PhotonEvent *cheat_photons = 0;
	Detector* d2 = 0;
	readInCheatData(particle_Tfile_p, particle_file, particle_tree, originals, cheat_photons, d2);


	TFile* reconstruction_Tfile_p = 0;
	TTree* reconstruction_tree = 0;
	TrackRecons *reconstructions = 0;
	readInReconstructionData(reconstruction_Tfile_p, reconstruction_file, reconstruction_tree, reconstructions);

	auto &pars   = originals->Particles;
	auto &cheat_phos = cheat_photons->Photons;
  auto &recons = reconstructions->Recon;
	vector<int>& index = reconstructions->index;

	stringstream ss;
	auto filename = [&ss, &directory, &basename, &particle_search, &particle_compare, &momentum_range](string const input1, string const input2){
		ss.str(""); ss << directory << "/" << input1 << input2 << basename << "_"<< particle_search << "_as_" << particle_compare << "_p_" << momentum_range.at(0) << "_" << momentum_range.at(1) << "_" << ".root";
		return ss.str();
	};


	string input_to_filename;
	switch(plotType){
		case 1: input_to_filename = "_scatterplot_"; break;
		case 2: input_to_filename = "_timeprojection_"; break;
	}

	TFile particleEvents(filename("correct_identification", input_to_filename).c_str(), "recreate");
	unsigned particleEvents_count = 0;

	TFile misIdentificationEvents(filename("incorrect_identification", input_to_filename).c_str(), "recreate");
	unsigned misIdentificationEvents_count = 0;

	int nentries = reconstruction_tree->GetEntries();
	if (event_range_set){
		if (event_range.at(1) > nentries)
			event_range.at(1) = nentries;
	}

	if ( !event_range_set )
		event_range = {0, nentries};

	if (print) cout << "particle_search = " << particle_search << endl;
	if (print) cout << "particle_compare = " << particle_compare << endl;

	TH2D h("momentum vs intensity", "momentum vs intensity", 200, 0, 900, 10, 2, 3);

	for (unsigned entry = event_range.at(0); entry < event_range.at(1); ++entry){
		if (print) cout << "----------------Event " << entry << "------------\n";
		photon_tree->GetEntry(entry);
		particle_tree->GetEntry(entry);
		reconstruction_tree->GetEntry(entry);

    if (pars.empty()) continue;
		Reconstruction photon_reconstruction;
		ReconstructEvent(photon_reconstruction, photon_event, false);

		vector<ParticleOut> par_outs(pars.begin(), pars.end());

		bool added_Identification_Batch = false;
		bool added_misIdentification_Batch = false;
		if ((misIdentificationEvents_count == max_count) && (particleEvents_count == max_count))
				break;
		for (unsigned i = 0; i < pars.size(); ++i){
			if ((misIdentificationEvents_count == max_count) && (particleEvents_count == max_count))
				break;
			// cout << "\tparticle " << i << endl;

			if (added_Identification_Batch && added_misIdentification_Batch){
			 continue;
			}

			auto& par = pars.at(i);
			auto& phos = photon_reconstruction.Photons.at(i);
			if (print) cout << "photons: " << phos.size() << endl;
			// if (print) cout << "index size " << index.size() << endl;
			if (phos.size() != index.size()){
				cout << "index and photons of different size!\n";
				// cout << " photons size " << phos.size() << endl;
				continue;
			}
			auto& recon = recons.at(i);
			double momentum = par.CalculateMomentum();
			// cout << par.name << ": momentum = " << momentum << endl;
			if (phos.empty()) continue;

			if (!withinrange(momentum_range, momentum)) continue;

			static string best_fit;
			best_fit = recon.getBestFit(threshold);


			// for (unsigned _i = 0; _i < recon.size(); ++_i){
			// 	cout << recon.getNameAt(_i) << " : " << recon.getnSigmaThetaAt(_i) << endl;
			// }


			if (print) cout << "best fit = " << best_fit << endl;
			bool search_is_best_fit = (particle_search == best_fit);



			double intensity = recon.getIntegralAt(recon.getIndexOf(particle_search));
			h.Fill(intensity, momentum);


			bool passed_Identification;
			if (search_is_best_fit) passed_Identification = true;
			else passed_Identification = false;

			// bool passed_Identification = passConditions(matchcondition_cases, functions, par, recon, particle_search_index, threshold, print);
			if (!added_Identification_Batch && passed_Identification && (par.name == particle_search) && (particleEvents_count < max_count)){
				AddBatch(entry, i, particleEvents, par_outs, pars, photon_reconstruction.Photons, cheat_phos, index, recons, particle_compare, particleEvents_count, threshold, plotType, time_min, time_max);
				added_Identification_Batch = true;
			}


			bool compare_is_best_fit = (particle_compare == best_fit);

			bool passed_misIdentification;
			if (compare_is_best_fit) passed_misIdentification = true;
			else passed_misIdentification = false;

			// bool passed_misIdentification = passConditions(matchcondition_cases, functions, par, recon, particle_search_index, threshold, print);
			// if (!passed_Identification) cout << "==============HEYYYYY+++++++++++\n";
			if (!added_misIdentification_Batch && passed_misIdentification && (par.name == particle_search) && (misIdentificationEvents_count < max_count)){
				AddBatch(entry, i, misIdentificationEvents, par_outs, pars, photon_reconstruction.Photons, cheat_phos, index, recons, particle_compare, misIdentificationEvents_count, -threshold, plotType, time_min, time_max);
				added_misIdentification_Batch = true;
			}



			if (print) cout << "\t- " << particle_search << " as " << particle_search << " : " << misIdentificationEvents_count << endl ;
			if (print) cout << "\t- " << particle_search << " as " << particle_compare << " : " << particleEvents_count << endl ;
		}


	}

if (print) cout << "final incorrect count: = " << misIdentificationEvents_count << endl;
if (print) cout << "final correct count:   = " << particleEvents_count << endl;
cout << particleEvents.GetName() << endl;
cout << misIdentificationEvents.GetName() << endl;

stringstream ss1; ss1.str(""); ss1 << particle_search << "_intensity.root";

TCanvas C("C", "C", 1000, 800);
h.Draw("COLZ");
C.SaveAs(ss1.str().c_str());


photon_Tfile_p->cd();
photon_Tfile_p->Close();

particle_Tfile_p->cd();
particle_Tfile_p->Close();

reconstruction_Tfile_p->cd();
reconstruction_Tfile_p->Close();

particleEvents.cd();
particleEvents.Write();
particleEvents.Close();

misIdentificationEvents.cd();
misIdentificationEvents.Write();
misIdentificationEvents.Close();



	return 0;
}
