#include <vector>
#include <string>
#include <unordered_map>

#include "TFile.h"
#include "TTree.h"


#include "dirc_objects.h"
#include "event_parsers.h"
#include "printFits.h"
#include "flagConditions.h"

#include <tclap/CmdLine.h>
using namespace std;

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&)> flag_fun_map;

int main(int argc, char const *argv[])
{

string fit_dir;
string reconstructiondata;
string cheatdata;
vector< int > event_range;
bool event_range_set = false;
string matchsearch;
string falsesearch;
vector< int> flags;
bool print;
TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{
	TCLAP::ValueArg<std::string> reconstructileFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);
	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("","match-search","particle which is being matched for",false,"electron","string", cmd);
	TCLAP::ValueArg<std::string> falsesearchArg("","false-search","particle which is being used for the fake rate",false,"pion","string", cmd);

	TCLAP::ValueArg<std::string> fitDirectoryArg("f","fit-directory","Directory where fits will be stored",false,"fits","string", cmd);
	TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);

	TCLAP::MultiArg<int> flagsArg("c","flag-condition","sets the method by which matches will be determined."
		"\n\t\tcase 1: fit as search item (default electron)"
		"\n\t\tcase 2: momentum is greater than 1.5"
		"\n\t\tcase 2: sigma is less than 1.5",false,"int", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv);
	fit_dir = fitDirectoryArg.getValue();
	reconstructiondata = reconstructileFileArg.getValue();
	cheatdata = particleFileArg.getValue();
	matchsearch = matchsearchArg.getValue();
	falsesearch = falsesearchArg.getValue();
	flags = flagsArg.getValue();
	print = verboseArg.getValue();
	event_range_set = eventRangeArg.isSet();
	if ( event_range_set ){
		event_range = eventRangeArg.getValue();
		if ( event_range.at(1) < event_range.at(0)){
			cout << "ERROR: The second value for the range must be higher than the first value.\n\nExiting!\n";
			return 0;
		}
	}
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	fit_dir.append("/");

	//------------------------------
	flag_fun_map flag_funcs;
	flag_funcs[1] = &momentum_exceeds_threshold;
	flag_funcs[2] = &correct_searchtype;
	flag_funcs[3] = &within_sigma_threshold;
	//------------------------------

	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;

	string file1 = cheatdata;
	string file2 = reconstructiondata;

	TFile f1(file1.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &originals);

	TFile f2(file2.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	int nentries = t1->GetEntries();
	if ( !event_range_set ){
		event_range.push_back(0);
		event_range.push_back(nentries);
	}
	TCanvas C("C", "C", 1000, 600);
	vector<Particle> *pars = &originals->Particles;
	vector<TrackRecon> *recons = &reconstructions->Recon;
	for (unsigned ev = 0; ev < nentries; ++ev){
		if (print) cout << "Event = " << ev << endl;
		t1->GetEntry(ev);
		t2->GetEntry(ev);
		if (ev > event_range[1]) break;
		if (ev < event_range[0]) continue;
		for (unsigned int p = 0; p < recons->size(); ++p){
			Particle* P = &pars->at(p);
			TrackRecon* R = &recons->at(p);
			printfits(C, ev, p, *P, *R, fit_dir, flag_funcs, flags);
		}
	}
return 0;
}

