#include "TGraphAsymmErrors.h"
#include "TH1D.h"
#include "TFile.h"
#include "TTree.h"

#include "event_parsers.h"
#include "match_conditions.h"
#include <tclap/CmdLine.h>
#include "utility_library.h"
#include "dirc_objects.h"

using namespace std;

typedef std::unordered_map<int, bool(*)(const Particle&, const TrackRecon&, const int&, const double&)> func_map;
typedef std::unordered_map<int, TH1D* > TH1D_map;
typedef std::unordered_map<int, std::vector< double > > vector_map;

class graphlabels
{
public:
	graphlabels(int i) : graphtype(i) {
		switch (graphtype){
			case 1: // multiplicity
				xtitle = "multiplicity";
			case 2: // momentum
				xtitle = "momentum";
		}
	}
	~graphlabels(){}

	void labelTGraph(TGraph*& G){
		G->SetTitle(title.c_str());
		G->GetXaxis()->SetTitle(xtitle.c_str());
		G->GetYaxis()->SetTitle(ytitle.c_str());
	}

	void setTitle(string match, string against){
		title = wul::appendStrings("success rate for identifying ", match, " as ", against);
	}
	void setYtitle(string yt){
		ytitle = yt;
	}
	int graphtype;

	string filename;
	string title;
	string xtitle;
	string ytitle;
};


void printTGraphASymError(TH1D& NumHist, TH1D& DenHist, const string& search, const string& dir, const string& basename, bool printhists){

	int numcounter = NumHist.GetEntries();
	int dencounter = DenHist.GetEntries();

	string numfilename = wul::appendStrings(dir, search, "_num_", basename);
	if (printhists) NumHist.SaveAs(numfilename.c_str(), "update");

	string denfilename = wul::appendStrings(dir, search, "_den_", basename);
	if (printhists) DenHist.SaveAs(denfilename.c_str(), "update");

	TGraphAsymmErrors graph;

	if (!(dencounter)){
		if (numcounter)
			cout << "The denominator is empty but the numerator isn't\n";
		return;
	}

	string graphname = wul::appendStrings(dir, search,basename);

	graph.BayesDivide(&NumHist, &DenHist);
	graph.SetMarkerStyle(20);
	graph.Draw("AP");
	double max = graph.GetMaximum();
	graph.GetYaxis()->SetRangeUser(0.,max);
	graph.SaveAs(graphname.c_str(), "update");
	cout << "graphname: " << graphname << endl;
}

void fillTH1Dmaps(TH1D_map& num, TH1D_map& den, const vector<int>& graph_choices, vector_map& bounds_map, const int& nbins){

	TH1D* H = 0;
	for (auto graph: graph_choices){
		auto& bounds = bounds_map[graph];
		auto& low = bounds[0];
		auto& hi = bounds[1];
		stringstream ss; ss << "graph "<< graph << " " << low << " : " << hi << " numerator";
		num[graph] = new TH1D(ss.str().c_str(), ss.str().c_str(), nbins, bounds[0], bounds[1]);
		ss.str(""); ss << "graph "<< graph << " " << low << " : " << hi << " denominator";
		den[graph] = new TH1D(ss.str().c_str(), ss.str().c_str(), nbins, bounds[0], bounds[1]);
	}
}