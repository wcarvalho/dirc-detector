#include "efficiencyheader.h"

int main()
{	

	string pre = "";
	string data_dir = "";
	string graph_dir = "graphs/";
	string fit_dir = "fits/";
	string fitdata = "fitresults.root";
	string cheatdata = "cheat.root";

	string matchgraph_filebase = "match";
	string falsegraph_filebase = "false-identification";

	string matchsearch = "electron";
	string falsesearch = "pion";
	
	double pi = TMath::Pi();

	fillparameters("directories", pre, data_dir, fit_dir, graph_dir);
	vector< vector< double > > momentum = filearray("momentum");
	vector< vector< double > > multiplicity = filearray("multiplicity");
	vector< vector< double > > settings = filearray("settings");
	double threshold = settings.at(0).at(0);
	int nptBins = int(settings.at(0).at(1));
	// vector< double > events = settings.at(1);
	bool print = settings.at(1).at(0); // row 2, 1st value
	bool makefits = settings.at(1).at(1); // row 2, 2nd value
	vector< double > event_range; event_range.clear(); 
	event_range.push_back(settings.at(2).at(0)); event_range.push_back(settings.at(2).at(1)); 


	vector< pair<int, pair <Particle, TrackRecon> > > numMatch; numMatch.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > denMatch; denMatch.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > numFalse; numFalse.clear();
	vector< pair<int, pair <Particle, TrackRecon> > > denFalse; denFalse.clear();

	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;
	
	
	data_dir = appendStrings(pre, data_dir);
	fit_dir = appendStrings(data_dir, fit_dir);

	string file1 = appendStrings(pre, cheatdata);
	string file2 = appendStrings(data_dir, fitdata);

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
		cout << "Event " << ev << endl;
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

	cout << "Finished primary Sort\n";

	vector< pair<int, pair <Particle, TrackRecon> > > numMatch_cut; numMatch_cut.clear();
	clearOuterParticles(numMatch, numMatch_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > denMatch_cut; denMatch_cut.clear();
	clearOuterParticles(denMatch, denMatch_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > numFalse_cut; numFalse_cut.clear();
	clearOuterParticles(numFalse, numFalse_cut, 490, 3.5, .2);

	vector< pair<int, pair <Particle, TrackRecon> > > denFalse_cut; denFalse_cut.clear();
	clearOuterParticles(denFalse, denFalse_cut, 490, 3.5, .2);

	cout << "\n--------Finished Making Copies without Edge Cutters\n\n";

	int multiplicity_bin_hi = multiplicity_high+1;
	int multiplicity_bin_low = 0;
	int multiplicity_bins = (multiplicity_high+multiplicity_low+1)*1000;

	int filenumber = 0;

	if (print) cout << "Making vs. Multiplicity Plots\n";

	makePlots(C, momentum, "Multiplicity", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, multiplicity_bin_low, multiplicity_bin_hi, multiplicity_bins, numMatch, denMatch, numFalse, denFalse, 1, filenumber, print);
	cout << "\n--------Finished vs. Multiplicity Plots\n\n";

	if (print) cout << "Making vs. Momentum Plots\n";
	makePlots(C, multiplicity, "Momentum", "multiplicity", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, momentum[0][0], momentum.back().back(), nptBins, numMatch, denMatch, numFalse, denFalse, 2, filenumber, print);
	cout << "\n--------Finished vs. Momentum Plots\n\n";

	if (print) cout << "Making vs. Incident Angle Plots\n";
	makePlots(C, momentum, "Incident Theta", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, 0, pi/2, 100, numMatch, denMatch, numFalse, denFalse, 3, filenumber, print);

	if (print) cout << "Making vs. Incident Angle Plots\n";
	makePlots(C, momentum, "Incident Phi", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, -pi, pi, 100, numMatch, denMatch, numFalse, denFalse, 4, filenumber, print);
	cout << "\n--------Finished vs. Incident Angle Plots\n\n";
/* ------------------------------------------------------
										 With Cuts
------------------------------------------------------ */ 
	makePlots(C, momentum, "Multiplicity (with Edge Cutters Removed)", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, multiplicity_bin_low, multiplicity_bin_hi, multiplicity_bins, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 1, filenumber, print);
	cout << "\n--------Finished vs. Multiplicity Plots (with Edge Cutters Removed)\n\n";
	
	makePlots(C, multiplicity, "Momentum (with Edge Cutters Removed)", "multiplicity", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, momentum[0][0], momentum.back().back(), nptBins, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 2, filenumber, print);
	cout << "\n--------Finished vs. Momentum Plots (with Edge Cutters Removed)\n\n";

	if (print) cout << "Making vs. Incident Angle Plots\n";
	makePlots(C, momentum, "Incident Theta (with Edge Cutters Removed)", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, 0, pi/2, 100, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 3, filenumber, print);
	makePlots(C, momentum, "Incident Phi (with Edge Cutters Removed)", "pt", data_dir, graph_dir, matchgraph_filebase, falsegraph_filebase, -pi, pi, 100, numMatch_cut, denMatch_cut, numFalse_cut, denFalse_cut, 4, filenumber, print);
	cout << "\n--------Finished vs. Incident Angle Plots (with Edge Cutters Removed)\n\n";

	return 0;
}