#include <sstream>
#include <string>
#include <iomanip>
#include <unordered_map>

#include "TKey.h"
#include "TMath.h"
#include "dircTH1D.h"
#include "TAxis.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <tclap/CmdLine.h>

using namespace std;
using namespace dirc;


double emission_angle(double const& P, double const mass){
	static double beta; beta = P/pow(( mass*mass + P*P ),.5);
	return TMath::ACos(1./(1.474*beta));
}

bool checkValid(TFile const& f){
	if (!(f.IsOpen())) {
		cout << f.GetName() << " invalid!\n";
		return false;
	}
	else return true;
}

bool getHistogram(TFile*& f, string& file, dircTH1D& H){
	bool histogram_is_okay;
	f = new TFile(file.c_str());
	histogram_is_okay = checkValid(*f);
	if (!histogram_is_okay) return false;
	TIter nextkey(f->GetListOfKeys());
	TKey *key = (TKey*)nextkey();
	dircTH1D* H_temp = (dircTH1D*)key->ReadObj();
	H = *H_temp;
	return true;
}

void calculate_error(double const& numerator, double const& denominator, double& error ){
	if ( (denominator < 1) || (numerator < 1) ){
		error = 0;
	}
	else
	error = 1/denominator * sqrt( numerator* ( 1 - numerator/denominator) );
}

void findEfficiencyBounds(dircTH1D& h, double const& center, double percent, double& xlow, double& xhi, double& percent_achieved, double& error, bool print){
		static double width;
		// double width_guess;
		// width_guess = sqrt( h.GetBinContent(h.GetMaximumBin()) );

		h.defineDistributionRange(h.GetXaxis()->GetXmin(), h.GetXaxis()->GetXmax());
		percent_achieved = h.defineSigma(center, 0., percent, print);

		width = h.getWidth();
		xlow = center - width;
		xhi = center + width;

		static double integral;
		integral = h.Integral(0, h.GetNbinsX());
		calculate_error(integral*percent_achieved, integral, error);
		percent_achieved *= 100;
		error *=  100;
}

void percentInRange(dircTH1D& h, double const& xlow, double const& xhi, double& false_rate, double& error){
	static int binlow;
	static int binhi;

	binlow = h.FindBin(xlow);
	binhi = h.FindBin(xhi);

	static double numerator;
	numerator = h.Integral(binlow, binhi);

	binlow = h.FindBin(xlow);
	binhi = h.FindBin(xhi);

	static double denominator;
	denominator = h.Integral(0, h.GetNbinsX());
	false_rate = numerator/denominator*100;

	if ( (denominator < 1) || (numerator < 1) ) false_rate = 0;
	calculate_error(numerator, denominator, error);
	error *= 100;
}


void plotFvsE(TGraphErrors*& g, dircTH1D& effh, dircTH1D& falseh, vector<double> const& efficiency_points, double const& efficiency_center, double const& momentum, double& max_falserate, bool print = false){



	static unsigned count;
	count = 0;
	static double old_efficiency; old_efficiency = 0.;
	static double old_false; old_false = 0.;
	static double efficiency_found; efficiency_found = 0.;
	static double efficiency_error; efficiency_error = 0.;
	static double false_rate; false_rate = 0.;
	static double false_error; false_error = 0.;
	for (double efficiency: efficiency_points){

		static double angle_low;
		static double angle_hi;

		findEfficiencyBounds(effh, efficiency_center, efficiency/100., angle_low, angle_hi, efficiency_found, efficiency_error, print);


		percentInRange(falseh, angle_low, angle_hi, false_rate, false_error);
		if (print) cout << momentum << ", "<< efficiency << " : " << setprecision(3) << efficiency_found << " \u00B1 " << efficiency_error << ", " << false_rate << " \u00B1 " << false_error << endl;
		if (fabs(efficiency_found - efficiency) >= 10){
			if (print) cout << "\tskipping\n";
			g->SetPoint(count, old_efficiency, old_false);
			++count;
			continue;
		}
		// if (print) cout << "\tbounds = " << angle_low << " : " << angle_hi << endl;

		if (max_falserate < false_rate) max_falserate = false_rate;
		g->SetPoint(count, efficiency_found, false_rate);
		g->SetPointError(count, efficiency_error, false_error);
		++count;
		old_efficiency = efficiency_found;
		old_false = false_rate;
		// if (efficiency > 70) break;
	}

}

template <typename T>
void deletenews(vector<T*>& v){
	for (auto& i: v )
		delete i;
}

void closeFiles(vector<TFile*>& files){
	for (auto& f: files){
		f->cd();
		f->Close();
	}
}

int main(int argc, char** argv){

	vector<string> efficiencyHists;
	vector<string> falserateHists;
	vector<double> momenta;
	vector<double> eff_points { 10., 20., 30., 40., 50., 60., 70., 80., 90., 95. };

	bool print;
	string output;
	string efficiencyType;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::MultiArg<std::string> efficiencyHistsArg("e","efficiencyHists","",true,"string", cmd);

	TCLAP::MultiArg<std::string> falserateHistsArg("f","falseHists","",true,"string", cmd);

	TCLAP::MultiArg<double> momentaArg("p","momenta","",true,"string", cmd);

	TCLAP::MultiArg<double> eff_pointsArg("","eff","efficiency points used",false,"string", cmd);

	TCLAP::ValueArg<std::string> outputArg("o","output","output-file",false,"theta_distribution.root","string", cmd);
	TCLAP::ValueArg<std::string> efficiencyTypeArg("t","efficiency-type","efficiency-type",false,"pion","string", cmd);

	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv );

	efficiencyHists = efficiencyHistsArg.getValue();
	falserateHists = falserateHistsArg.getValue();
	momenta = momentaArg.getValue();
	output = outputArg.getValue();
	efficiencyType = efficiencyTypeArg.getValue();
	print = verboseArg.getValue();
	if (eff_pointsArg.isSet()) eff_points = eff_pointsArg.getValue();


}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	unsigned neffHists = efficiencyHists.size();
	unsigned nfakeHists = falserateHists.size();
	unsigned nmomenta = momenta.size();

	if ( (neffHists != nfakeHists) && (nfakeHists != nmomenta) ){
		cout << "ERROR: Please give the same number of histograms and momenta for each argument\n";
		exit(1);
	}

	unordered_map < string, double> masses;
	masses["muon"] = .105658;
	masses["electron"] = .511e-3;
	masses["pion"] = .1396;
	masses["kaon"] = .493667;
	masses["proton"] = .938;

	string title="false-positive vs. efficiency";
	TCanvas C(title.c_str(), title.c_str(), 1000, 800 );
	C.SetGrid();

	TLegend L(0.1,0.5,0.2,0.9, "momentum");
	L.SetTextSize(.025);


	vector<TFile*> effFiles;
	vector<TFile*> falseFiles;
	vector<TGraphErrors* > graphs;
	vector<dircTH1D* > effHists;
	vector<dircTH1D* > falseHists;

	TGraphErrors* g = 0;
	TFile* effFile = 0;
	TFile* falseFile = 0;

	static vector<int> colors {20, 27, 28, 29, 30, 31, 34, 38};
	int color = 0;

	double max_falserate = 0.;

	double mass = masses[efficiencyType];

	for (unsigned i = 0; i < neffHists; ++i){
		++color;
		auto& p = momenta.at(i);
		static double efficiency_center; efficiency_center = emission_angle(p, mass);
		effFile = new TFile();
		falseFile = new TFile();

		dircTH1D effHist;
		dircTH1D falseHist;

		g = new TGraphErrors(eff_points.size());

		auto& efficiencyHist_file = efficiencyHists.at(i);
		bool histogram_good = getHistogram (effFile, efficiencyHist_file, effHist);

		auto& falserateHist_file = falserateHists.at(i);
		histogram_good *= getHistogram (falseFile, falserateHist_file, falseHist);

		if (!histogram_good) continue;
		plotFvsE(g, effHist, falseHist, eff_points, efficiency_center, p, max_falserate, print);

		graphs.push_back(std::move(g));
		// effHists.push_back(std::move(effHist));
		// falseHists.push_back(std::move(falseHist));
		if ( (color == 10) || (color == 5) ) ++color;
		graphs.back()->SetMarkerColor(color);
		graphs.back()->SetLineColor(color);
		graphs.back()->SetMarkerSize(1.25);
		graphs.back()->SetMarkerStyle(8);
		graphs.back()->SetLineWidth(1);
		C.cd();

		if (color == 1){
			graphs.back()->SetTitle(title.c_str());
			graphs.back()->GetXaxis()->SetTitle("efficiency");
			graphs.back()->GetYaxis()->SetTitle("false-positive rate");
			graphs.back()->Draw("ALP");
		}
		else{
			graphs.back()->Draw("LP");
		}


		static stringstream ss;
		ss.str(""); ss << p;

		L.AddEntry(graphs.back(), ss.str().c_str(), "mp");
		// effFile->cd(); effFile->Close();
		// falseFile->cd(); falseFile->Close();
		effFiles.push_back(std::move(effFile));
		falseFiles.push_back(std::move(falseFile));
	}

	graphs.at(0)->GetYaxis()->SetRangeUser(0, max_falserate + 5);
	L.Draw();
	C.Print(output.c_str());


	closeFiles(effFiles);
	closeFiles(falseFiles);
	deletenews(graphs);
}
