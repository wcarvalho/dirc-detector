#include <set>
#include <unordered_map>
#include <sstream>

#include "new_efficiency.h"

int main(int argc, char** argv){

	string graph_dir;
	string calibration_dir;
	string reconstructiondata;
	string cheatdata;

	string matchgraph_filebase;
	string falsegraph_filebase;

	string matchsearch;
	string falsesearch;

	vector<double> momentum_slices {0., 3.};
	vector<double> multiplicity_slices {0., 5.};

	double threshold = 0.;
	int nptBins = 0;
	bool print; // row 2, 1st value
	vector< int > event_range;
	event_range.push_back(-1);
	event_range.push_back(-1);
	vector< int > graph_choice;
	vector<int> graph_types;
	vector<int> matchcondition_cases = {2, 4};
	vector<string> graph_names {"momentum.root", "multiplicity.root"};
	bool plotefficiency;
	bool plotfalsepositive;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{


	TCLAP::ValueArg<std::string> match_fakeDirectoryArg("D","directory","Directory where graphs for match rates and fake rated will be stored",false,"graphs","string", cmd);


	TCLAP::ValueArg<std::string> reconstructileFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);

	TCLAP::ValueArg<std::string> matchPrefixArg("","match-prefix","prefix used for the match-rate graph file names",false,"match","string", cmd);

	TCLAP::ValueArg<std::string> falsePrefixArg("","false-prefix","prefix used for the fake-rate graph file names",false,"false","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("M","match-search","particle which is being matched for",false,"electron","string", cmd);

	TCLAP::ValueArg<std::string> falsesearchArg("F","false-search","particle which is being used for the fake rate",false,"pion","string", cmd);

	TCLAP::ValueArg<double> thresholdArg("t","threshold","value at which delta sigma values are cut",false, 1,"double", cmd);

	TCLAP::ValueArg<double> ptbinsArg("","ptbins","number of pt bins",false, 100,"double", cmd);

	TCLAP::MultiArg<int> matchconditionArg("m","match-condition","sets the method by which matches will be determined."
		"\n\t\tcase 1: within_expectedphoton_threshold"
		"\n\t\tcase 2: inside_box"
		"\n\t\tcase 3: inside_circle"
		"\n\t\tcase 4: xyplane",false, "double", cmd);

	TCLAP::MultiArg<double> momentum_slicesArg("","moms","momentum slices",false,"string", cmd);

	TCLAP::MultiArg<double> multiplicity_slicesArg("","muls","multiplicity slices",false ,"string", cmd);


	// TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "double", cmd);


	TCLAP::MultiArg<int> graphsChoiceArg("c","graphs-choice","determines which graphs are drawn. 1: multiplicity, 2: momentum, 3: theta, 4: phi ", false, "int", cmd);

	TCLAP::MultiArg<std::string> graphsNameArg("","graph-names","sets the name of the graphs to be printed. The match and false type will be prepended to each filename", false, "string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg plotefficiencyArg("e","efficiency","print efficiency plot", cmd, false);
	TCLAP::SwitchArg plotfalsepositiveArg("f","false-positive","print false-positive plot", cmd, false);
	cmd.parse( argc, argv );
	graph_dir = match_fakeDirectoryArg.getValue();
	reconstructiondata = reconstructileFileArg.getValue();
	cheatdata = particleFileArg.getValue();

	matchgraph_filebase = matchPrefixArg.getValue();
	falsegraph_filebase = falsePrefixArg.getValue();

	matchsearch = matchsearchArg.getValue();
	falsesearch = falsesearchArg.getValue();
	threshold = thresholdArg.getValue();
	nptBins = ptbinsArg.getValue();
	print = verboseArg.getValue();
	plotefficiency = plotefficiencyArg.getValue();
	plotfalsepositive = plotfalsepositiveArg.getValue();

	if (momentum_slicesArg.isSet())
		momentum_slices = momentum_slicesArg.getValue();

	if (multiplicity_slicesArg.isSet())
		multiplicity_slices = multiplicity_slicesArg.getValue();

	if (matchconditionArg.isSet())
		matchcondition_cases = matchconditionArg.getValue();

	if ( graphsChoiceArg.isSet() )
		graph_choice = graphsChoiceArg.getValue();

	if ( graphsNameArg.isSet() )
		graph_names = graphsNameArg.getValue();


}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	graph_dir.append("/");

	double pi = TMath::Pi();
	gErrorIgnoreLevel = 5000;					// turn off all root printing

	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;

	string file1 = cheatdata;
	string file2 = reconstructiondata;

	TCanvas C("C", "C", 1000, 600);

	TFile f1(file1.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &originals);
	TFile f2(file2.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	vector<Particle> *pars = &originals->Particles;
	vector<TrackRecon> *recons = &reconstructions->Recon;

	func_map functions;
	functions[1] = &within_expectedphoton_threshold;
	functions[2] = &inside_box;
	functions[3] = &inside_circle;
	functions[4] = &xyplane;

	vector_map bounds_map;
	bounds_map[1] = multiplicity_slices;
	bounds_map[2] = momentum_slices;

	++nptBins; // bins is for histogram whihch will have npbins -1 centers. centers used as points
	TH1D_map matchnumerators;
	TH1D_map matchdenominators;
	fillTH1Dmaps(matchnumerators, matchdenominators, graph_choice, bounds_map, nptBins);

	TH1D_map falsenumerators;
	TH1D_map falsedenominators;
	fillTH1Dmaps(falsenumerators, falsedenominators, graph_choice, bounds_map, nptBins);

	// reference a particulat histogram first by index for type, then by which slice it belongs to for that

	int multiplicity = 0;
	auto getEventMultiplicity = [&multiplicity](TTree &t1, TTree &t2, std::vector<TrackRecon>& recons,std::vector<Particle>& pars, bool print){
		multiplicity = pars.size();
		dirc::matchDataSize(t1, t2, recons, pars, print);
	};

	int nmatch = 0;
	int nfalse = 0;
	auto fillHistogram = [&threshold, &functions, &graph_choice, &multiplicity, &matchcondition_cases](TrackRecon& r, Particle& p, string& search, TH1D_map& numerator, TH1D_map& denominator, const double& momentum, bool print){

		for (unsigned i = 0; i < r.Options.size(); ++i){
			string name = r.Options.at(i);
			if (!(name == search)) continue;
			bool passed = passed_conditions(p, r, i, threshold, functions, matchcondition_cases);
			for (auto choice: graph_choice){
				switch (choice){
					case 1:
						denominator[choice]->Fill(multiplicity);
						if (passed) numerator[choice]->Fill(multiplicity);
						break;
					case 2:
						denominator[choice]->Fill(momentum);
						if (passed) numerator[choice]->Fill(momentum);
						break;
				}
			}
		}
	};

	auto fillHistograms = [&nfalse, &nmatch, &plotefficiency, &plotfalsepositive, &fillHistogram, &matchnumerators, &matchdenominators, &falsenumerators, &falsedenominators, &matchsearch, &falsesearch](TrackRecon& r, Particle& p, bool print){

		if (!(plotefficiency || plotfalsepositive)) return;
		double momentum = p.CalculateMomentum();

		if (plotefficiency){
			if (p.name != matchsearch) return;
			++nmatch;
			fillHistogram(r, p, matchsearch, matchnumerators, matchdenominators, momentum, print);
		}
		if (plotfalsepositive) {
			if (p.name != falsesearch) return;
			++nfalse;
			fillHistogram(r, p, matchsearch, falsenumerators, falsedenominators, momentum, print);
		}

	};

	int nentries = t1->GetEntries();
	int firstevent = 0;

	dirc::parseEvents(*t1, *t2, *originals, *reconstructions, firstevent, nentries,
		getEventMultiplicity, dirc::true_eventcondition,
		fillHistograms, dirc::true_trialcondition,
		dirc::empty_eventparser, dirc::true_eventcondition,
		print);
	if (print) cout << nmatch << " " << matchsearch << "s" << endl;
	if (print) cout << nfalse << " " << falsesearch << "s" << endl;

	// graphlabels matchlabels("efficiency", "momentum", wul::appendString("identifying ", matchsearch, " as ", matchsearch);

	for (unsigned i = 0; i < graph_choice.size(); ++i){
		int index = graph_choice.at(i);
		if (plotefficiency) printTGraphASymError(*matchnumerators[index], *matchdenominators[index], matchsearch, graph_dir, graph_names.at(i));
		if (plotfalsepositive) printTGraphASymError(*falsenumerators[index], *falsedenominators[index],falsesearch, graph_dir, graph_names.at(i));
	}

	for (auto i: graph_choice){
		delete matchnumerators[i];
		delete matchdenominators[i];
		delete falsenumerators[i];
		delete falsedenominators[i];
	}

}