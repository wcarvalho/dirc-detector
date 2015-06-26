#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include <fstream>
#include "TFile.h"
#include "FileProperties.h"
#include "TGraphAsymmErrors.h"
#include "TGraphErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TAxis.h"
#include "TKey.h"
#include <sstream>
#include <tclap/CmdLine.h>

using namespace std;

class data_point
{
public:
	data_point(){}
	data_point(double const x, double const y, double const xe, double const ye){
		setPoint(x,y,xe,ye);
	}
	~data_point(){}

	void setPoint(double const x, double const y, double const xe, double const ye){
		__x = x;
		__y = y;
		__x_err = xe;
		__y_err = ye;
	}

  friend std::ostream& operator<<(std::ostream& os, data_point const& p){
    os << p.x() << " \u00B1 " << p.x_err() << ", " << p.y() << " \u00B1 " << p.y_err();
    return os;
	}
	friend data_point operator+(data_point const& p1, data_point const& p2){
		double p_x = p1.x() + p2.x();
		double p_y = p1.y() + p2.y();
		double p_x_err = p1.x_err() + p2.x_err();
		double p_y_err = p1.y_err() + p2.y_err();
		data_point dp(p_x, p_y, p_x_err, p_y_err);
		return dp;
	}

	friend data_point operator*(double const cons, data_point const& p){
		data_point dp(p.__x*cons, p.__y*cons, p.__x_err*cons, p.__y_err*cons);
		return dp;
	}

	double x() const { return __x; }
	double y() const { return __y; }
	double x_err() const { return __x_err; }
	double y_err() const { return __y_err; }

// private:
	double __x;
	double __y;
	double __x_err;
	double __y_err;

};

typedef vector< data_point > data_points;
// typedef vector<pair <double,double> > xy_t;
// typedef unordered_map<double, xy_t > data_t;
typedef unordered_map<double, data_points > data_t;
// typedef map<double, xy_t > ordered_data_t;
typedef map<double, data_points > ordered_data_t;

void stringsIntoVector(string file, vector<string> &v);
void checkValid(const TFile& f);
string getGraphName(string& file);
void getGraph(string& file, TGraphAsymmErrors*& Plot);
void extractGraphData(TGraphAsymmErrors matches, TGraphAsymmErrors fakes,data_t& data, double& highestx, double& highesty);
void MakeGraph(const double& momentum, data_points const& points, int graphnumber, TGraphErrors*& g);


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

	double highestx {0.};
	double highesty {0.};

	for (unsigned i = 0; i < matches.size(); ++i){
		getGraph(matches.at(i), matchPlot);
		getGraph(fakes.at(i), fakePlot);
		extractGraphData(*matchPlot, *fakePlot, data, highestx, highesty);
	}

	ordered_data_t ordered_data(data.begin(), data.end());
	TCanvas C("C", "C", 1000, 600);
	C.SetGrid();
	TLegend L(0.1,0.5,0.2,0.9, "momentum");
	L.SetTextSize(.025);
	vector < TGraphErrors* > graphs;
	int count = 0;
	TGraph g;
	g.SetPoint(0, 0, 0);
	g.SetPoint(1, highestx, highesty);
	g.SetTitle("False-Positive Rate vs. Efficiency");
	g.GetXaxis()->SetTitle("Efficiency (% of electrons identified as electrons)");
	g.GetYaxis()->SetTitle("False-Positive Rate (% of pions identified as electrons)");
	g.Draw("AP");
	TGraphErrors *G = 0;

	auto i = ordered_data.begin();
	double p1 = i->first;
	++i; double p2 = i->first;
	double p_step = p2-p1;
	double p_half_step = p_step/2;

	for (auto i = ordered_data.begin(); i != ordered_data.end(); ++i){
		const double& momentum = i->first;
		auto& dps = i->second;

		G = new TGraphErrors();
		MakeGraph(momentum, dps, count, G);
		graphs.push_back(std::move(G));
		graphs.back()->Draw("P");
		stringstream ss; ss.str("");
		ss << std::setprecision(3) << (momentum - p_half_step) << " : " << std::setprecision(3)<< (momentum + p_half_step);
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
	dirc::FileProperties fp(file);
	return std::move(fp.get_filename_without_suffix());
}
void getGraph(string& file, TGraphAsymmErrors*& Plot){
	TFile f(file.c_str()); checkValid(f);
	TIter nextkey(f.GetListOfKeys());
	TKey *key = (TKey*)nextkey();
	Plot = (TGraphAsymmErrors*)key->ReadObj();
}
void extractGraphData(TGraphAsymmErrors matches, TGraphAsymmErrors fakes,data_t& data, double& highestx, double& highesty){

	int npoints = matches.GetN();
	if (matches.GetN() != fakes.GetN()){
		cout << "Matches: " << matches.GetN() << endl;
		cout << "Fakes: " << fakes.GetN() << endl;
		cout << "incompatible\n"; exit(1);
	}

	for (unsigned i = 0; i < npoints; ++i){
		static double effx = 0., effy = 0.;
		static double fakex = 0., fakey = 0.;
		static double eff_error = 0., fake_error = 0.;

		matches.GetPoint(i, effx, effy);
		eff_error = matches.GetErrorY(i);

		fakes.GetPoint(i, fakex, fakey);
		fake_error = matches.GetErrorY(i);
		double &pt = effx;

		data_point dp(effy, fakey, eff_error, fake_error);
		dp = 100*dp;

		effy=dp.x();
		fakey=dp.y();
		if (highestx < effy) highestx = effy;
		if (highesty < fakey) highesty = fakey;

		// pair<double,double> FE(effy,fakey);
		data[pt].push_back(std::move(dp));
		// cout << pt << ": x = " << effy << ", y = " << fakey << endl;
	}
}

void MakeGraph(const double& momentum, data_points const& points, int graphnumber, TGraphErrors*& g){

	stringstream ss; ss.str("");
	ss << "momentum=" << momentum;
	g->SetName(ss.str().c_str());

	for (unsigned i = 0; i < points.size(); ++i){
		data_point const& point = points.at(i);
		double x = point.x();
		double x_error = point.x_err();
		double y = point.y();
		double y_error = point.y_err();
		g->SetPoint(i, x, y);
		g->SetPointError(i, x_error, y_error);
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