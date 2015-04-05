#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include "TFile.h"
#include "FileProperties.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TKey.h"
#include <sstream>
#include <tclap/CmdLine.h>

using namespace std;

typedef vector<pair <double,double> > xy_t;
typedef unordered_map<double, xy_t > data_t;
typedef map<double, xy_t > ordered_data_t;

void stringsIntoVector(string file, vector<string> &v);
void checkValid(const TFile& f);
string getGraphName(string& file);
void getGraph(string& file, TGraphAsymmErrors*& Plot);
void extractGraphData(TGraphAsymmErrors matches, TGraphAsymmErrors fakes,data_t& data);
void MakeGraph(const double& pt, const xy_t& xy, int graphnumber, TGraph*& g);

int main(int argc, char const *argv[])
{

string matchfile;
string fakefile;
string outputfile;
bool print;
TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{
	TCLAP::ValueArg<std::string> matchFileArg("m","match-file","text file with list of matches",true,"matches.txt","string", cmd);
	TCLAP::ValueArg<std::string> fakeFileArg("f","fake","text file with list of fakes",true,"fakes.txt","string", cmd);
	TCLAP::ValueArg<std::string> outputFileArg("o","output","filename of graph to be printed (will be root file)",true,"fakevseff.root","string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv);
	matchfile = matchFileArg.getValue();
	fakefile = fakeFileArg.getValue();
	outputfile = outputFileArg.getValue();
	print = verboseArg.isSet();
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }
	vector<string> fakes;
	stringsIntoVector(fakefile, fakes);

	vector<string> matches;
	stringsIntoVector(matchfile, matches);

	data_t data;

	TGraphAsymmErrors* matchPlot;
	TGraphAsymmErrors* fakePlot;

	for (unsigned i = 0; i < matches.size(); ++i){
		getGraph(matches.at(i), matchPlot);
		getGraph(fakes.at(i), fakePlot);
		extractGraphData(*matchPlot, *fakePlot, data);
	}

	ordered_data_t ordered_data(data.begin(), data.end());
	TCanvas C("C", "C", 1000, 600);
	C.SetGrid();
	TLegend L(0.1,0.5,0.2,0.9, "pt");
	L.SetTextSize(.025);
	vector < TGraph* > graphs;
	int count = 0;
	TGraph g;
	g.SetPoint(0, 0, 0);
	g.SetPoint(1, 1, .8);
	g.SetTitle("Fake Rate vs. Efficiency");
	g.GetXaxis()->SetTitle("Efficiency (electrons identified as electrons)");
	g.GetYaxis()->SetTitle("Fake Rate (pions identified as electrons)");
	g.Draw("AP");
	TGraph *G = 0;
	for (auto i = ordered_data.begin(); i != ordered_data.end(); ++i){
		const double& pt = i->first;
		xy_t& xy = i->second;

		G = new TGraph();
		MakeGraph(pt, xy, count, G);
		graphs.push_back(std::move(G));
		graphs.back()->Draw("P");
		stringstream ss; ss.str("");
		ss << pt;
	 	L.AddEntry(graphs.back(),ss.str().c_str(),"lp");
		++count;
	}

	L.Draw();
	C.Print(outputfile.c_str());

	delete G;

}

void stringsIntoVector(string file, vector<string> &v){
	ifstream f(file.c_str());
	if (!(f.good())){	cout << "bad file!\n"; exit(1); }
	static string s;
	while (f.good()){
		f >> s;
		if (!(f.good())) break;
		v.push_back(s);
		s.clear();
	}
}
void checkValid(const TFile& f){
	if (!(f.IsOpen())) {
		cout << f.GetName() << " invalid!\n";
		exit(1);
	}
}
string getGraphName(string& file){
	wul::FileProperties fp(file);
	return std::move(fp.get_filename_without_suffix());
}
void getGraph(string& file, TGraphAsymmErrors*& Plot){
	TFile f(file.c_str()); checkValid(f);
	TIter nextkey(f.GetListOfKeys());
	TKey *key = (TKey*)nextkey();
	Plot = (TGraphAsymmErrors*)key->ReadObj();
}
void extractGraphData(TGraphAsymmErrors matches, TGraphAsymmErrors fakes,data_t& data){

	int npoints = matches.GetN();
	if (matches.GetN() != fakes.GetN()){
		cout << "Matches: " << matches.GetN() << endl;
		cout << "Fkaes: " << fakes.GetN() << endl;
		cout << "incompatible\n"; exit(1);
	}

	for (unsigned i = 0; i < npoints; ++i){
		static double effx = 0., effy = 0.;
		static double fakex = 0., fakey = 0.;

		matches.GetPoint(i, effx, effy);
		fakes.GetPoint(i, fakex, fakey);
		double &pt = effx;

		pair<double,double> FE(effy,fakey);
		data[pt].push_back(FE);
		// cout << pt << ": x = " << effy << ", y = " << fakey << endl;
	}
}

void MakeGraph(const double& pt, const xy_t& xy, int graphnumber, TGraph*& g){

	stringstream ss; ss.str("");
	ss << "pt=" << pt;
	g->SetName(ss.str().c_str());

	for (unsigned i = 0; i < xy.size(); ++i){
		const double&x = xy.at(i).first;
		const double&y = xy.at(i).second;
		g->SetPoint(i, x, y);
	}
	g->SetMarkerStyle(22);
	// g->SetMarkerSize(1);
	int markercolor = 2*(graphnumber)+1;
	g->SetMarkerColor(markercolor);
	g->SetLineColor(markercolor);

	TFile f("graphs.root", "update");
	g->Write();
	f.Close();
}