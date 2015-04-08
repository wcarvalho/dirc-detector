#include "dirc_objects.h"
#include "particle_objects.h"
#include "photon_objects.h"
#include "analysis_objects.h"
#include <algorithm>
#include "TMath.h"
#include "TVirtualFitter.h"
#include "iomanip"
using namespace std;

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
	for (unsigned int i = 0; i < data.size(); i++)
		h1.Fill(data.at(i).at(which));
	h1.SetDefaultSumw2();
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