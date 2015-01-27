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
	for (unsigned int i = 0; i < data.size(); i++)
		h2.Fill(data.at(i).at(0), data.at(i).at(1));
	h2.SetDefaultSumw2();
	Hists2D.push_back(h2);
}

void Fit::NumPar(int npar)
{
	std::vector<TH1D> temp1D; temp1D.clear();
	std::vector<TH2D> temp2D;	temp2D.clear();
	std::vector<double> tempdub; tempdub.clear();
	for (unsigned int i = 0; i < npar; ++i)
	{
		Event1D.push_back(temp1D);
		Event2D.push_back(temp2D);
		Candidates1D.push_back(tempdub);
	}
}

void Fit::Add1DFit(TCanvas* c, TH1D hist, int par, double anglex, int option)
{
	double sig = .1;
	double xlow = anglex-sig;
	double xhigh = anglex+sig;
	TH1D *h = &hist;
	string name1 = h->GetName();

	stringstream fname;
	fname << "_anglex_" << anglex;
	name1.append(fname.str());
	string name2 = name1; string name3 = name2; 
	
	name1.append("1");
	name2.append("2");
	name3.append("3");

  stringstream function;
  function << "exp( -(x-" << anglex << ")*(x-"<<anglex<<")/(2.*"<<.01<<"*"<<.01<<"))++ 1 ++ x ++ x*x";
	TF1 f1(name1.c_str(), function.str().c_str() , xlow, xhigh);
	h->Fit(&f1, "QR");

	c->Print(name1.append(".png").c_str());

	TF1 f2(name2.c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3] + [4]*x + [5]*x*x", xlow, xhigh);
	f2.SetParameter(0,f1.GetParameter(0));
	f2.SetParameter(1,anglex);
	f2.SetParameter(2,.01);
	for (unsigned int i =3; i < 6; ++i) f2.SetParameter(i,f1.GetParameter(i-2));
	h->Fit(&f2, "QR"); 	h->Fit(&f2, "QR"); 	h->Fit(&f2, "QR");

	TF1 f3(name3.c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) )", xlow, xhigh);
	for (unsigned int i=0; i<3;i++) f3.SetParameter(i,f2.GetParameter(i));

		// getparameter for again

	double numberofphotons;
	numberofphotons =	f3.Integral(xlow, xhigh)/h->GetBinWidth(1);
	if (print == true )cout << "\t\tnumberofphotons in integral = " << numberofphotons << endl;

	h->Draw();
	c->Print(name2.append(".png").c_str());
	
	Event1D.at(par).push_back(*h);
	Hists1D.push_back(*h);
}

void Fit::Add2DFit(TH2D hist, int par, double anglex, double angley)
{
	Event2D.at(par).push_back(hist);
	TH2D &h = Event2D.at(par).back();
}

int ClosestAngle(double angle, std::vector<double> angles)
{
	double min_value;
	int min_element;
	for (unsigned int i =0; i < angles.size(); ++i)
	{
		angles.at(i) = fabs(angles.at(i)-angle);
	}
	
	min_value = *std::min_element(angles.begin(), angles.end());
	
	for (unsigned int i =0; i < angles.size(); ++i)
		if (min_value == angles.at(i)) min_element = i;
	
	return min_element;	
}

void Fit::Find1DCandidates(int nfound, double xpeaks[], int par, std::vector<double> angles)
{
	double probangle = 0;
	for (unsigned int i = 0; i < nfound; ++i)
	{
		int probable_element = ClosestAngle(xpeaks[i], angles); 
		probangle = angles.at(probable_element);
		if (fabs(probangle-xpeaks[i]) < .005)
			Candidates1D.at(par).push_back(probangle);
	}
}