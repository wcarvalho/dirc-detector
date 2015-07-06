#include <sstream>
#include <string>


#include "TKey.h"
#include "dircTH1D.h"
#include "TAxis.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TGraphErrors.h"
#include "TLegend.h"

#include <tclap/CmdLine.h>

using namespace std;
using namespace dirc;

void checkValid(TFile const& f){
	if (!(f.IsOpen())) {
		cout << f.GetName() << " invalid!\n";
		exit(1);
	}
}

void getHistogram(TFile*& f, string& file, dircTH1D& H){
	f = new TFile(file.c_str()); checkValid(*f);
	TIter nextkey(f->GetListOfKeys());
	TKey *key = (TKey*)nextkey();
	dircTH1D* H_temp = (dircTH1D*)key->ReadObj();
	H = *H_temp;
}

void findEfficiencyBounds(dircTH1D& h, double const& center, double percent, double& xlow, double& xhi){
		static double width;
		double width_guess;
		width_guess = sqrt( h.GetBinContent(h.GetMaximumBin()) );

		h.defineDistributionRange(h.GetXaxis()->GetXmin(), h.GetXaxis()->GetXmax());
		h.defineSigma(center, width_guess, percent);

		width = h.getWidth();
		xlow = center - width;
		xhi = center + width;
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

	static double numerator_error;
	static double denominator_error;

	// numerator_error = sqrt(numerator);
	// denominator_error = sqrt(denominator);
	error = 1/denominator * sqrt( numerator* ( 1 - numerator/denominator) );
	// error = pow((numerator*numerator + denominator*denominator), .5);


}

void plotFvsE(TGraphErrors*& g, dircTH1D& effh, dircTH1D& falseh, vector<double> const& efficiency_points, double const& momentum, double& max_falserate){

	static double center;
	center = effh.GetMean();

	static unsigned count;
	count = 0;
	for (double efficiency: efficiency_points){

		static double angle_low;
		static double angle_hi;

		findEfficiencyBounds(effh, center, efficiency/100., angle_low, angle_hi);

		static double false_rate;
		static double false_error;
		percentInRange(falseh, angle_low, angle_hi, false_rate, false_error);
		cout << momentum << " : " << efficiency << ", " << false_rate << endl;

		if (max_falserate < false_rate) max_falserate = false_rate;
		g->SetPoint(count, efficiency, false_rate);
		++count;
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

	string output;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::MultiArg<std::string> efficiencyHistsArg("e","efficiencyHists","",true,"string", cmd);

	TCLAP::MultiArg<std::string> falserateHistsArg("f","falseHists","",true,"string", cmd);

	TCLAP::MultiArg<double> momentaArg("p","momenta","",true,"string", cmd);

	TCLAP::MultiArg<double> eff_pointsArg("","eff","efficiency points used",false,"string", cmd);

	TCLAP::ValueArg<std::string> outputArg("o","output","output-file",false,"theta_distribution.root","string", cmd);

	cmd.parse( argc, argv );

	efficiencyHists = efficiencyHistsArg.getValue();
	falserateHists = falserateHistsArg.getValue();
	momenta = momentaArg.getValue();
	output = outputArg.getValue();
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


	string title="false-positive vs. efficiency (kaons as pions)";
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

	int color = 1;

	double max_falserate = 0.;

	for (unsigned i = 0; i < neffHists; ++i){
		auto& p = momenta.at(i);

		effFile = new TFile();
		falseFile = new TFile();

		dircTH1D effHist;
		dircTH1D falseHist;

		g = new TGraphErrors(eff_points.size());

		auto& efficiencyHist_file = efficiencyHists.at(i);
		getHistogram (effFile, efficiencyHist_file, effHist);

		auto& falserateHist_file = falserateHists.at(i);
		getHistogram (falseFile, falserateHist_file, falseHist);

		plotFvsE(g, effHist, falseHist, eff_points, p, max_falserate);

		graphs.push_back(std::move(g));
		// effHists.push_back(std::move(effHist));
		// falseHists.push_back(std::move(falseHist));
		if (color == 10) ++color;

		graphs.back()->SetMarkerColor(color);
		graphs.back()->SetMarkerSize(.75);
		graphs.back()->SetMarkerStyle(8);
		graphs.back()->SetLineColor(color);
		graphs.back()->SetLineWidth(3);
		++color;
		C.cd();

		if (i == 0){
			graphs.back()->SetTitle(title.c_str());
			graphs.back()->GetXaxis()->SetTitle("efficiency");
			graphs.back()->GetYaxis()->SetTitle("false-positive rate");
			graphs.back()->Draw("ALP");
		}
		else{
			graphs.back()->Draw("SAME");
		}

		static stringstream ss;
		ss.str(""); ss << p;

		L.AddEntry(graphs.back(), ss.str().c_str(), "lp");
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
