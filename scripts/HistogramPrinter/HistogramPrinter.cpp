#include <vector>
#include <string>
#include <unordered_map>

#include "TFile.h"
#include "TTree.h"
#include "TError.h"

#include "dirc_objects.h"
#include "event_parsers.h"
#include "passConditions.h"
#include "HistogramPrinter.h"

#include <tclap/CmdLine.h>
using namespace std;

int main(int argc, char const *argv[])
{
string directory;
string cheatdata;
string reconstructiondata;
string matchsearch;
string falsesearch;
string output_base;

vector< int > event_range;
vector< int> flags;

bool event_range_set = false;
bool print;
bool print1D;
bool print1Dfit;
bool print2Dfit;
bool colored;

double threshold;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{
	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);
	TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> outputFileArg("o","output","file base. 1D, 2D, and fits will be appended depending on which histogram is printed",false,"histograms","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("","match-search","particle which is being matched for",false,"electron","string", cmd);
	TCLAP::ValueArg<std::string> falsesearchArg("","false-search","particle which is being used for the fake rate",false,"pion","string", cmd);

	TCLAP::ValueArg<double> thresholdArg("t","threshold","",false,1.,"double", cmd);

	TCLAP::ValueArg<std::string> fitDirectoryArg("f","fit-directory","Directory where fits will be stored",false,"fits","string", cmd);

	TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);

	TCLAP::MultiArg<int> flagsArg("c","flag-condition","sets the method by which matches will be determined."
		"\n\t\tcase 1: momentum is greater than momentum-threshold"
		"\n\t\tcase 2: fit as search item (default electron)"
		"\n\t\tcase 3: sigma is less than sigma-threshold",false,"int", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg print2DfitArg("","f2D","print 2D histograms with fits", cmd, false);
	TCLAP::SwitchArg print1DArg("","p1D","print 1D histograms", cmd, false);
	TCLAP::SwitchArg print1DfitArg("","f1D","print 1D histograms with fits", cmd, false);
	TCLAP::SwitchArg coloredArg("","colored","print 2D histograms as colored canvases", cmd, false);
	cmd.parse( argc, argv);
	directory = fitDirectoryArg.getValue();
	reconstructiondata = reconstructionFileArg.getValue();
	cheatdata = particleFileArg.getValue();
	matchsearch = matchsearchArg.getValue();
	falsesearch = falsesearchArg.getValue();
	output_base = outputFileArg.getValue();
	threshold = thresholdArg.getValue();
	flags = flagsArg.getValue();
	print = verboseArg.getValue();
	print1D = print1DArg.getValue();
	print1Dfit = print1DfitArg.getValue();
	print2Dfit = print2DfitArg.getValue();
	colored = coloredArg.getValue();

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
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }
	directory.append("/");

	vector<int> fit_conditions{2, 3};
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
	if ( !event_range_set )
		event_range = {0, nentries};

	string TH1Dfilename = wul::appendStrings(directory, output_base, "TH1D.root");
	string TH2Dfilename = wul::appendStrings(directory, output_base, "TH2D.root");

	gErrorIgnoreLevel = 5000;
  if (print) gErrorIgnoreLevel = 0;         // turn off all root printing
	TCanvas C("C", "C", 1000, 600);
	TH1D* h1 = 0;
	TH2D* h2 = 0;


	auto printHistogram = [&C, &fit_conditions, &flags, &flag_funcs, &threshold, &TH1Dfilename, &TH2Dfilename, &colored, &h1, &h2, &print1D, &print1Dfit, &print2Dfit](TrackRecon& R, Particle& P, bool print){

		if (find(flags.begin(), flags.end(), 1) != flags.end())
			if (!momentum_exceeds_threshold(P, R, 1, false)) return;

		TFile TH2Df(TH2Dfilename.c_str(), "update");
		h2 = &R.Hist2D;
		h2->SetStats(0);
		if (colored){
			h2->Draw("colz");
			C.Write(h2->GetName());
			C.Clear();
		}
		else h2->Write();
		TH2Df.Close();

		h1 = h2->ProjectionY();

		if (print1D)
			print1DHistogram(C, *h1, P, R, flag_funcs, flags, threshold, TH1Dfilename, print1Dfit);

	};


	dirc::parseEvents(*t1, *t2, *originals, *reconstructions,
		event_range[0], event_range[1],
    dirc::matchDataSize, dirc::true_eventcondition,
    printHistogram, dirc::true_trialcondition,
    dirc::empty_eventparser, dirc::true_eventcondition,
    print);

	cout << "1D file: " << TH1Dfilename << endl;
	cout << "2D file: " << TH2Dfilename << endl;
return 0;
}

