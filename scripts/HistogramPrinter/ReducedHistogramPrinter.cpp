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
#include "reconstructor.h"
#include "createScatterPlot.h"

#include <tclap/CmdLine.h>
using namespace std;

int main(int argc, char const *argv[])
{
string directory;
string photondata;
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
	TCLAP::ValueArg<std::string> photonFileArg("P","photon-file","file with photon data",false,"photons.root","string", cmd);
	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);
	TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> outputFileArg("o","output","file base. 1D, 2D, and fits will be appended depending on which histogram is printed",false,"histograms","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("","match-search","particle which is being matched for",false,"electron","string", cmd);
	TCLAP::ValueArg<std::string> falsesearchArg("","false-search","particle which is being used for the fake rate",false,"pion","string", cmd);

	TCLAP::ValueArg<double> thresholdArg("t","threshold","",false,1.,"double", cmd);

	TCLAP::ValueArg<std::string> fitDirectoryArg("f","fit-directory","Directory where fits will be stored",false,"fits","string", cmd);

	TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "int", cmd);

	TCLAP::MultiArg<int> flagsArg("c","flag-condition","sets the method by which matches will be determined."
		"\n\t\tcase 1: momentum is greater than 1.5"
		"\n\t\tcase 2: fit as search item (default electron)"
		"\n\t\tcase 3: sigma is less than threshold",false,"int", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	TCLAP::SwitchArg print2DfitArg("","f2D","print 2D histograms with fits", cmd, false);
	TCLAP::SwitchArg print1DArg("","p1D","print 1D histograms", cmd, false);
	TCLAP::SwitchArg print1DfitArg("","f1D","print 1D histograms with fits", cmd, false);
	TCLAP::SwitchArg coloredArg("","colored","print 2D histograms as colored canvases", cmd, false);
	cmd.parse( argc, argv);
	directory = fitDirectoryArg.getValue();
	reconstructiondata = reconstructionFileArg.getValue();
	cheatdata = particleFileArg.getValue();
	photondata = photonFileArg.getValue();
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

  Reconstruction photon_reconstruction;
  GeneratorOut* photon_event = 0;
  TFile file(photondata.c_str(), "read");
	TTree *photon_ttree = (TTree*)file.Get("sim_out");
	photon_ttree->SetBranchAddress("simEvent", &photon_event);

	int nentries = t1->GetEntries();
	if ( !event_range_set )
		event_range = {0, nentries};

	gErrorIgnoreLevel = 5000;
  if (print) gErrorIgnoreLevel = 0;         // turn off all root printing

	string filename2D = dirc::appendStrings(directory, output_base, "_TH2Dreduced_plot.root");
	string filename1D = dirc::appendStrings(directory, output_base, "_TH1Dreduced_plot.root");
  auto &pars   = originals->Particles;
  auto &recons = reconstructions->Recon;
	vector<int>& index = reconstructions->index;
	TCanvas C("C", "C", 1000, 600);
	for (unsigned ev = 0; ev < nentries; ++ev){
		t1->GetEntry(ev);
		t2->GetEntry(ev);
		photon_ttree->GetEntry(ev);

		cout << ev << endl;
		dirc::matchDataSize(*t1, *t2, recons, pars, print);
		ReconstructEvent(photon_reconstruction, photon_event, print);

		vector<ParticleOut> par_outs(pars.begin(), pars.end());
		if (pars.empty() || recons.empty()) continue;
		int j = 0;
		for(unsigned i = 0; i < pars.size(); ++i){
			auto& phos = photon_reconstruction.Photons.at(i);
			if ( phos.empty() ) continue;
			if (phos.size() != index.size() ) {
				cout << "phos.size() = " << phos.size() << endl;
				cout << "index.size() = " << index.size() << endl;
				cout << "ERROR with index or reconstruction!\n"; return 0;
			}
			auto& recon = recons.at(i);
			auto& par = pars.at(i);
			if (find(flags.begin(), flags.end(), 1) != flags.end())
				if (!momentum_exceeds_threshold(par, recon, 1, false)) continue;
			auto& h2 = recon.Hist2D;
			string histname = h2.GetName();
			createIndexedPhotonScatterPlot(par_outs, phos, index, i, histname, filename2D.c_str() , "update");

			if (!print1D) continue;
			string h1histname;
			double xlow;
			double xhi;
			int nbins;
			h1histname = dirc::appendStrings(histname, "1D");
			xlow = h2.GetXaxis()->GetXmin();
			xhi = h2.GetXaxis()->GetXmax();
			nbins = h2.GetNbinsX();
			TH1D* h1 = CreateReducedHistogram(phos, index, i, h1histname, nbins, xlow, xhi);
			h1->SetDefaultSumw2();
			print1DHistogram(C, *h1, par, recon, flag_funcs, flags, threshold, filename1D, print1Dfit);
			delete h1;
		}
	}

	cout << "file 1D: " << filename1D << endl;
	cout << "file 2D: " << filename2D << endl;

return 0;
}

