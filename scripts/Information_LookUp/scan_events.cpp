#include "scan_events.h"
#include <tclap/CmdLine.h>

using namespace std;

int main(int argc, char const *argv[])
{
	string particle_file;
	string reconstruction_file;

	string particle_search;
	string particle_compare;

	double threshold;


	vector<int> matchcondition_cases = {1, 4};
	vector<double> momentum_range {1.75, 2.25};
	vector< int > event_range;

	bool event_range_set = false;
	bool print = false;

	TCLAP::CmdLine cmd("Options for printing", ' ', "0.1");
	try{
		TCLAP::ValueArg<string> particle_fileArg("p", "particle-file", "particle information", false, "particle_file.root", "string", cmd);
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

		TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);

		cmd.parse( argc, argv);
		particle_file = particle_fileArg.getValue();
		reconstruction_file = reconstruction_fileArg.getValue();
		particle_search = particle_searchArg.getValue();
		particle_compare = particle_compareArg.getValue();

		threshold = thresholdArg.getValue();

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

	flag_fun_map functions;
	functions[1] = &within_angle_resolution;
	functions[2] = &inside_box;
	functions[3] = &inside_circle;
	functions[4] = &xyplane;


	TFile particle_Tfile(particle_file.c_str(), "read"); dirc::checkValid(particle_Tfile);
	TTree* particle_tree = (TTree*)particle_Tfile.Get("cheat_info");
	ParticleEvent *originals = 0;
	particle_tree->SetBranchAddress("Particle Event", &originals);

	TFile reconstruction_Tfile(reconstruction_file.c_str(), "read"); dirc::checkValid(reconstruction_Tfile);
	TTree* reconstruction_tree = (TTree*)reconstruction_Tfile.Get("identifications");
	TrackRecons *reconstructions = 0;
	reconstruction_tree->SetBranchAddress("guesses", &reconstructions);


	auto &pars   = originals->Particles;
  auto &recons = reconstructions->Recon;

	int nentries = particle_tree->GetEntries();
	if ( !event_range_set )
		event_range = {0, nentries};

	vector<string> event_matches;

	unsigned event = 0;
	unsigned particle = 0;
	auto get_event = [&event, &particle](TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){
		++event;
		particle = 0;
		dirc::matchDataSize(t1, t2, recons, pars, print);
	};

	auto find_event_matches = [&event_matches, &event, &momentum_range, &particle_search, &particle_compare, &functions, &matchcondition_cases, &threshold, &particle](TrackRecon& r, Particle& p, bool print){
		++particle;
		string name = p.GetName();
		if (name != particle_search) return;

		double momentum = p.CalculateMomentum();
		if (!dirc::withinrange(momentum_range, momentum)) return;

		int reconstruction_index = getReconIndex(r, particle_compare);
		if (reconstruction_index == -1) return;

		bool match = passed_as_bad_event(p, r, reconstruction_index, threshold, functions, matchcondition_cases);
		static stringstream ss;
		if (match){
			ss.str(""); ss << "event " << event << ", particle " << particle << endl;
			event_matches.push_back(ss.str());
		}
	};

	dirc::parseEvents(*particle_tree, *reconstruction_tree, *originals, *reconstructions,
		event_range[0], event_range[1],
    get_event, dirc::true_eventcondition,
    find_event_matches, dirc::true_trialcondition,
    dirc::empty_eventparser, dirc::true_eventcondition,
    print);

	for (auto& i: event_matches)
		cout << i << endl;
	return 0;
}
