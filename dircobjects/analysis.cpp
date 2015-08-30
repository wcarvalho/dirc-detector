#include "dirc_objects.h"
#include "particle_objects.h"
#include "photon_objects.h"
#include "analysis_objects.h"
#include <algorithm>
#include "TMath.h"
#include "TList.h"
#include "TVirtualFitter.h"
#include "iomanip"

void TrackRecon::clear(){
	Options.clear();
	Sigmas.clear();
	Areas.clear();
	ExpectedNumber.clear();
	Params.clear();
	Funs.clear();
}

void TrackRecon::printLatest(){
	for (unsigned int i = 0; i < 20; ++i)
		cout << "-";
	cout << "\nLatest Reconstruction Data\n";
	cout << "\tOption = " << Options.back() << endl;
	cout << "\tdSigma = " << Sigmas.back() << endl;
	cout << "\tArea = " << Areas.back() << endl;
	cout << "\tExpectedNumber = " << ExpectedNumber.back() << endl;
	cout << "\tParam = ";
	for (unsigned int i = 0; i < Params.back().size(); ++i){
		cout << Params.back().at(i) << "   ";
	}
	cout << endl;
	for (unsigned int i = 0; i < 20; ++i)
		cout << "-";
	cout << endl;
	// cout << "Funs = " << Funs.back() << endl;
}

int TrackRecon::getIndexOf(std::string type) const{
	for (int i = 0; i < size(); ++i){
		if (getNameAt(i) == type) return i;
	}
	return -1;
}

bool TrackRecon::passed_intensity_cut(int const i, bool print, double threshold) const {

	static bool passed;
	static double nsigma; nsigma = getnSigmaAreaAt(i, print);
	static double intensity; intensity = getIntegralAt(i);

	static bool nsigma_less_than_intensity_threshold;
	nsigma_less_than_intensity_threshold = (fabs(nsigma) <= threshold);
	static bool intensity_high_enough;
	intensity_high_enough = (intensity >= 40);

	// passed = nsigma_less_than_intensity_threshold && intensity_high_enough;
	passed = intensity_high_enough;

	return passed;
}
std::string TrackRecon::getBestFit(double const threshold, bool print) const {

	static double lowestsigma; lowestsigma = 1.e10;
	static double nsigma;
	static string bestfit; bestfit = "";
	static string name;
	for (unsigned i = 0; i < size(); ++i){
		name = getNameAt(i);
		nsigma = fabs(getnSigmaAt(i, print));
		if ((nsigma < lowestsigma) && (nsigma < threshold)) {
			lowestsigma = nsigma;
			bestfit = name;
		}
	}
	if (print) cout << "\t>>>best fit: " << bestfit << " with \u0394\u03C3 " << lowestsigma << endl;
	return bestfit;
}
void TrackRecon::addFitsToHistogram(TH1D &h){

	if (Params.empty()) return;

	for (int i = 0; i < size(); ++i){
		TF1* f1 = new TF1(getNameAt(i).c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
		for (unsigned int j = 0; j < 4; j++)
				f1->SetParameter(j, Params.at(i).at(j));

		const double& xlow = Params.at(i).at(4);
		const double& xhi = Params.at(i).at(5);
		f1->SetRange(xlow, xhi);
		f1->SetLineColor(i+1);
		h.GetListOfFunctions()->Add(f1);
	}
}
void TrackRecon::addFitToHistogram(TH1D &h, std::string type){

	if (Params.empty()) return;

	int i = getIndexOf(type);
	if (i < 0) return;
	TF1* f1 = new TF1(getNameAt(i).c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3]");
	for (unsigned int j = 0; j < 4; j++)
			f1->SetParameter(j, Params.at(i).at(j));

	const double& xlow = Params.at(i).at(4);
	const double& xhi = Params.at(i).at(5);
	f1->SetRange(xlow, xhi);
	f1->SetLineColor(2);
	h.GetListOfFunctions()->Add(f1);
}

double TrackRecon::getnSigmaAt(int const i, bool const print) const{

	static double nsigma;
	if (!passed_intensity_cut(i, print)) nsigma = 100;
	else nsigma = getnSigmaThetaAt(i, print);
	if (print) cout << "\t" << getNameAt(i) << " final \u0394\u03C3 = " << nsigma << endl << endl;
	return nsigma;
}

double TrackRecon::getnSigmaOf(std::string type, bool const print) const{

	static int i; i = getIndexOf(type);
	return std::move(getnSigmaAt(i, print));
}





void TrackRecons::AddTrackRecon(){
	TrackRecon tr;
	Recon.push_back(tr);
}
void TrackRecons::PushBackParams(){
	vector<double> temp;
	Recon.back().Params.push_back(temp);
}








void Analysis::AddTH1D(const char* name, const char* title, int nbinsx, double xlow, double xup, int which)
{
	TH1D h1(name, title, nbinsx, xlow, xup);
	h1.SetDefaultSumw2();
	for (unsigned int i = 0; i < data.size(); i++)
		h1.Fill(data.at(i).at(which));
	Hists1D.push_back(h1);
}

void Analysis::AddTH2D(const char* name, const char* title, int nbinsx, double xlow, double xup, int nbinsy, double ylow, double yup)
{
	TH2D h2(name, title, nbinsx, xlow, xup, nbinsy, ylow, yup);
	h2.SetDefaultSumw2();
	for (unsigned int i = 0; i < data.size(); i++)
		h2.Fill(data.at(i).at(0), data.at(i).at(1));
	Hists2D.push_back(h2);
}