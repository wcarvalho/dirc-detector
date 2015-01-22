#include "efficiencyheader.h"
#include "../scripts.h"
#include <tclap/CmdLine.h>

int main(int argc, char** argv)
{	

	string graph_dir;
	string fit_dir;
	string reconstructiondata;
	string cheatdata;
	string matchgraph_filebase;
	string falsegraph_filebase;
	string matchsearch;
	string falsesearch;
	string momentum_slices;
	string multiplicity_slices;
	double threshold = 0.;
	int nptBins = 0;
	bool print; // row 2, 1st value
	bool makefits=true;
	vector< int > event_range;
	event_range.push_back(-1);
	event_range.push_back(-1);
	vector< int > graph_choice;
	
TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{   

	TCLAP::ValueArg<std::string> fitDirectoryArg("f","fit-directory","Directory where fits will be stored",false,"fits","string", cmd);

	TCLAP::ValueArg<std::string> match_fakeDirectoryArg("m","match-fake-directory","Directory where graphs for match rates and fake rated will be stored",false,"graphs","string", cmd);

	TCLAP::ValueArg<std::string> reconstructileFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);

	TCLAP::ValueArg<std::string> matchPrefixArg("","match-prefix","prefix used for the match-rate graph file names",false,"match","string", cmd);

	TCLAP::ValueArg<std::string> falsePrefixArg("","false-prefix","prefix used for the fake-rate graph file names",false,"false","string", cmd);

	TCLAP::ValueArg<std::string> matchsearchArg("","match-search","particle which is being matched for",false,"electron","string", cmd);

	TCLAP::ValueArg<std::string> falsesearchArg("","false-search","particle which is being used for the fake rate",false,"pion","string", cmd);

	TCLAP::ValueArg<double> thresholdArg("t","threshold","value at which delta sigma values are cut",false, 1.5,"double", cmd);

	TCLAP::ValueArg<double> ptbinsArg("","ptbins","number of pt bins",false, 100,"double", cmd);

	TCLAP::ValueArg<std::string> momentum_slicesArg("","momentum-slices","file with momentum slices",false, "momentum","string", cmd);

TCLAP::ValueArg<std::string> multiplicity_slicesArg("","multiplicity-slices","file with multiplicity slices",false, "multiplicity","string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);
	
	TCLAP::MultiArg<int> eventRangeArg("e","event-range","events for which you want to print fits", false, "double", cmd);

	TCLAP::MultiArg<int> graphsChoiceArg("c","graphs-choice","determines which graphs are drawn. 1: multiplicity, 2: momentum, 3: theta, 4: phi ", false, "double", cmd);

	cmd.parse( argc, argv );
	fit_dir = fitDirectoryArg.getValue();
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
	momentum_slices = momentum_slicesArg.getValue();
	multiplicity_slices = multiplicity_slicesArg.getValue();
	if ( eventRangeArg.isSet() ){
		event_range = eventRangeArg.getValue();
		if ( event_range.at(1) < event_range.at(0)){
			cout << "The second value for the range must be higher than the first value.\n\nExiting!\n";
			return 0;
		}
	}
	if ( graphsChoiceArg.isSet() )
		graph_choice = graphsChoiceArg.getValue();


}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }
	

	fit_dir.append("/");
	graph_dir.append("/");

	
	double pi = TMath::Pi();

	vector< vector< double > > momentum = filearray(momentum_slices);
	vector< vector< double > > multiplicity = filearray(multiplicity_slices);

	vector< pair<int, pair <Particle, TrackRecon> > > numMatch; numMatch.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > denMatch; denMatch.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > numFalse; numFalse.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > denFalse; denFalse.clear();


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

	int nentries = t2->GetEntries();

	int i = 0;
	int multiplicity_high = 0;
	int multiplicity_low = 100;
	for (unsigned int ev = 0; ev < nentries; ++ev){
		if(print) cout << "Event " << ev << endl;
		t1->GetEntry(ev); t2->GetEntry(ev);
		int event_multiplicity = pars->size();
		if (multiplicity_high < event_multiplicity)
			multiplicity_high = event_multiplicity;
		if (multiplicity_low > event_multiplicity)
			multiplicity_low = event_multiplicity;
		
		int size_difference = matchDataSize(*recons, *pars);
		for (unsigned int p = 0; p < recons->size(); ++p){
			Particle* P = &pars->at(p);
			TrackRecon* R = &recons->at(p);
			if ((ev >= event_range[0])&&(ev <= event_range[1])){
				printfits(C, ev, p, *P, *R, fit_dir, makefits);
				// print = true;
			}
			else
				// print = false;

			++i;
			getMatch(P, matchsearch, matchsearch, threshold, R, event_multiplicity, numMatch, denMatch, print);				// fills the denominator with all of the electrons and the numerator with all the correct electron identifications 
			
			getMatch(P, falsesearch, matchsearch, threshold, R, event_multiplicity, numFalse, denFalse, print);				// fills the denominator with all of the pions and the numerator with all the false electron identifications
		}
	}

	// cout << "Finished primary Sort\n";

	vector< pair<int, pair <Particle, TrackRecon> > > numMatch_cut; numMatch_cut.clear();
	clearOuterParticles(numMatch, numMatch_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > denMatch_cut; denMatch_cut.clear();
	clearOuterParticles(denMatch, denMatch_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > numFalse_cut; numFalse_cut.clear();
	clearOuterParticles(numFalse, numFalse_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > denFalse_cut; denFalse_cut.clear();
	clearOuterParticles(denFalse, denFalse_cut, 490, 3.5, .2);

	// cout << "\n--------Finished Making Copies without Edge Cutters\n\n";

	int multiplicity_bin_hi = multiplicity_high+1;
	int multiplicity_bin_low = 0;
	int multiplicity_bins = (multiplicity_high+multiplicity_low+1)*1000;

	int filenumber = 0;


	if (std::find(graph_choice.begin(), graph_choice.end(), 1)!=graph_choice.end()){
		if (print) cout << "Making vs. Multiplicity Plots\n";
		makePlots(C, momentum, "multiplicity", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, multiplicity_bin_low, multiplicity_bin_hi, multiplicity_bins, numMatch, denMatch, numFalse, denFalse, 1, filenumber, print);
	}

	if (std::find(graph_choice.begin(), graph_choice.end(), 2)!=graph_choice.end()){
		if (print) cout << "Making vs. Momentum Plots\n";
		makePlots(C, multiplicity, "pt", "multiplicity", "", graph_dir, matchgraph_filebase, falsegraph_filebase, momentum[0][0], momentum.back().back(), nptBins, numMatch, denMatch, numFalse, denFalse, 2, filenumber, print);
	}
	
	if (std::find(graph_choice.begin(), graph_choice.end(), 3)!=graph_choice.end()){
		if (print) cout << "Making vs. Incident Angle Plots\n";
		makePlots(C, momentum, "incident_theta", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, 0, pi/2, 100, numMatch, denMatch, numFalse, denFalse, 3, filenumber, print);
	}

	if (std::find(graph_choice.begin(), graph_choice.end(), 4)!=graph_choice.end()){
		if (print) cout << "Making vs. Incident Angle Plots\n";
		makePlots(C, momentum, "incident_phi", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, -pi, pi, 100, numMatch, denMatch, numFalse, denFalse, 4, filenumber, print);
	}
/* ------------------------------------------------------
										 With Cuts
------------------------------------------------------ */ 
	return 0;
	makePlots(C, momentum, "Multiplicity (with Edge Cutters Removed)", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, multiplicity_bin_low, multiplicity_bin_hi, multiplicity_bins, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 1, filenumber, print);
	cout << "\n--------Finished vs. Multiplicity Plots (with Edge Cutters Removed)\n\n";
	
	makePlots(C, multiplicity, "pt (with Edge Cutters Removed)", "multiplicity", "", graph_dir, matchgraph_filebase, falsegraph_filebase, momentum[0][0], momentum.back().back(), nptBins, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 2, filenumber, print);
	cout << "\n--------Finished vs. Momentum Plots (with Edge Cutters Removed)\n\n";

	if (print) cout << "Making vs. Incident Angle Plots\n";
	makePlots(C, momentum, "Incident Theta (with Edge Cutters Removed)", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, 0, pi/2, 100, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 3, filenumber, print);
	makePlots(C, momentum, "Incident Phi (with Edge Cutters Removed)", "pt", "", graph_dir, matchgraph_filebase, falsegraph_filebase, -pi, pi, 100, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 4, filenumber, print);
	cout << "\n--------Finished vs. Incident Angle Plots (with Edge Cutters Removed)\n\n";

}