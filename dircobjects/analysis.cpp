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

int TrackRecon::getIndexOf(std::string type){
	for (int i = 0; i < size(); ++i){
		if (getNameAt(i) == type) return i;
	}
	return -1;
}

bool TrackRecon::passed_intensity_cut (int const i, double threshold = 7) const {

	static double nsigma; nsigma = getnSigmaAreaAt(i);
	// std::cout << "ExpectedNumber = " << ExpectedNumber.at(i)  << std::endl;
	// std::cout << "nsigmaArea = " << nsigma << std::endl;
	// std::cout << "threshold = " << threshold << std::endl;
	if (fabs(nsigma) <= threshold){
		// cout << "passed_intensity_cut\n";
		return true;
	}
	else{
		// cout << "didn't passed_intensity_cut\n";
		return false;
	}

}

std::string TrackRecon::getBestFit(double const threshold, bool print){

	static double lowestsigma; lowestsigma = 1.e10;
	static double nsigma;
	static string bestfit; bestfit = "";
	static string name;
	if (print) cout << "\tthreshold = " << threshold << endl;
	for (unsigned i = 0; i < size(); ++i){
		nsigma = fabs(getnSigmaThetaAt(i));
		name = getNameAt(i);
		if (print) cout << "\t" << name << " : " << nsigma << endl;
		if ((nsigma < lowestsigma) && (nsigma < threshold)) {
			lowestsigma = nsigma;
			bestfit = name;
		}
	}
	if (print) cout << "\t\tbest fit = " << bestfit << " with sigma = " << lowestsigma << endl;
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