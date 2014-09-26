#include <map>
#include <string>
#include "../headers/functions.h"
#include "../headers/fitter.h"

using namespace std;

double Identifier::FitParticle1D(TCanvas* c, TH1D &h, double xlow, double xhi, double center, double width, string name, bool make, bool print){

	h.SetName(name.c_str());

	stringstream function;
  function << "exp( -(x-" << center << ")*(x-"<<center<<")/(2.*"<<width<<"*"<<width<<"))++ 1 ++ x ++ x*x";
	
  string f1name = name;
  f1name.append("initial_fit");
	TF1 f1(f1name.c_str(), function.str().c_str() , xlow, xhi);
	h.Fit(&f1, "QR");
	TF1 f2(name.c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) ) + [3] + [4]*x + [5]*x*x", xlow, xhi);
	f2.SetParameter(0,f1.GetParameter(0));
	f2.SetParameter(1, center);
	f2.SetParLimits(1,center-0.005, center+0.005);
	f2.SetParameter(2, width);
	f2.SetParLimits(2,width*0.95, width*1.05);
	for (unsigned int i =3; i < 6; ++i){
		f2.SetParameter(i,f1.GetParameter(i-2));
	}

	h.Fit(&f2, "QBIR"); 

	string f3name = name;
	f3name.append("integral");
	TF1 f3(f3name.c_str(), "[0]*exp( -(x-[1])*(x-[1])/(2.*[2]*[2]) )", xlow, xhi);
	for (unsigned int i=0; i<3;i++) f3.FixParameter(i,f2.GetParameter(i));

	double numberofphotons =	f3.Integral(xlow, xhi)/h.GetBinWidth(1);
	
	string prefix = "../../Graphs/";
	prefix.append(name);
	
	h.GetXaxis()->SetRangeUser(xlow, xhi);
	h.Draw();
	if (make) {
		c->Print(prefix.append(".png").c_str());	
	}

	return numberofphotons;
}