#include "iostream"
#include <vector>
#include <cmath>
#include <string>
#include "TObject.h"
#include "../headers/fitter.h"
#include "../headers/simulator.h"
 
using namespace std;

bool Cut(double theta, double phi, double x, double y, double z){

	Detector d;
	Simulate simPho(theta, phi);
	simPho.SetStart(x, y, z);
	simPho.SetDim(d.Length, d.Width, d.Height);

	bool cut = false;
	for(unsigned int reflections = 0; reflections< 4; ++reflections){
		simPho.GotoWall(false);
		double &x_p = simPho.coord[0];
		if((x_p == 0 ) || (x_p == d.Length)){ break; } 
		double th = simPho.Theta;
		double ph = simPho.Phi;
		Photon photon(th, ph);
		photon.Wall = simPho.wall;
		d.get_Critical_Angle(1.);
		CheckAngel(d, photon, "no");
		if(photon.Flag){
			cut = true;
			break;
		}
		simPho.Reflect(false);
	}
	return cut;
}

double RiemannSum(double const& x, double const& y, double const& theta, double const& phi, double const& v){
	double pi = TMath::Pi();
	
	int total = 0;
	int passed = 0;

	Detector d;
	double l = d.Length;
	double w = d.Width;
	double h = d.Height;
	
	double emissionAngle = acos(1./(1.474*v));
	Rotater r;
	r.Feed_Particle(theta, phi);
	
	Simulate simPar(theta, phi);
	simPar.SetDim(l, w, h);
	simPar.SetStart(x, y, 0.);
	simPar.DistancetoWalls( );
	simPar.WhichWall( );
	double Path_length = simPar.WillTravel();
	simPar.Traveled = 0.;
	
	int PathSteps = 100;
	int PhiSteps = 50;
	
	double phi_measure = 0.;
	while(simPar.Traveled < Path_length){
		for(phi_measure = 0;  phi_measure < 2*pi; phi_measure += 2*pi/PhiSteps)
		{
			++total;
			Photon p(emissionAngle, phi_measure);
			double &th = p.Theta;
			double &ph = p.Phi;
			r.Rotate_Photon(th, ph);
			if (!Cut(th, ph, simPar.coord[0],simPar.coord[1],simPar.coord[2])) ++passed;
		}
		simPar.TravelDistance(Path_length/(double)PathSteps);
	}
	// cout << "passed = " << passed << endl;
	// cout << "total = " << total << endl;

	double xlow = 200e-9;
	double xhigh = 1000e-9;
	double z = 1.;
	double alpha = 1./137;
	double n = d.n;
	// cout << "d.n = " << d.n << endl;
	double nu = (1-(1/(v*v*n*n)));
	double Constant = 2*pi*alpha*nu*nu;
	TF1 f("dNdx", "1/x/x", xlow, xhigh);

	// cout << "\ttotal = " << total << endl;

	double percent_passed = double(passed)/total;
	// cout << "\tpercent passed = " << percent_passed << ": " << passed << endl;
	double dNdx = 1e-2*Constant*f.Integral(xlow, xhigh);
	double NPhotons = percent_passed*Path_length*dNdx;
	return NPhotons;
}

void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &madeit, double &traveled, bool print){
	
	// hard code l, w, h
	double pi = TMath::Pi();
	double Path_length = 0.;
	mass m(eta, pt);
	std::map<double,double> &atm = m.AngletoMass;
	Detector d(l,w,h);
	Rotater r;
	r.Feed_Particle(theta, phi);

	int total = 0;
	int passed = 0;
	int PathSteps = 100;
	int PhiSteps = 50;
	double phi_measure = 0.;

	Simulate simPar(theta, phi);
	simPar.SetDim(l, w, h);
	simPar.SetStart(x, y, 0);
	simPar.DistancetoWalls( );
	simPar.WhichWall( );
	Path_length = simPar.WillTravel();

	traveled = Path_length;
	for (std::map<double,double>::iterator it=atm.begin(); it!=atm.end(); ++it)
	{
		total = 0;
		passed = 0;
		simPar.Traveled = 0.;
		while(simPar.Traveled < Path_length){
			for(phi_measure = 0;  phi_measure < 2*pi; phi_measure += 2*pi/PhiSteps)
			{
				++total;
				Photon p(it->first, phi_measure);
				double &th = p.Theta;
				double &ph = p.Phi;
				r.Rotate_Photon(th, ph);
				if (!Cut(th, ph, simPar.coord[0],simPar.coord[1],simPar.coord[2])) ++passed;
			}
			simPar.TravelDistance(Path_length/PathSteps);
		}
		simPar.SetStart(x, y, 0);
		madeit[it->first] = double(passed)/total;
	}

}