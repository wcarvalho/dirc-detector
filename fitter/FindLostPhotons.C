#include "iostream"
#include <vector>
#include <cmath>
#include <string>
#include "TObject.h"
#include "dirc_objects.h"
#include "../headers/fitter.h"
#include "../headers/simulator.h"

using namespace std;
void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &madeit)
{

	ParticleOut p(theta, phi);
	double pi = TMath::Pi();
	double ThetaBeam = 2*atan(exp(-eta));
	double P = pt/sin(ThetaBeam);
	map<int, double> ThetaC; map<int, double>::iterator it;
		double thetaC_temp = 0;


	double beta_temp = 0;
	for (unsigned int par = 0; par < 5; par++)
	{
		beta_temp = P/pow((p.masses[par]*p.masses[par]+P*P),.5);
		thetaC_temp = acos(1./(1.474*beta_temp));
		if (thetaC_temp == thetaC_temp)
			ThetaC[par] = thetaC_temp;
	}
	Detector d(l,w,h);
	Rotater r;
	r.Feed_Particle(theta, phi);

	int total = 0;
	int passed = 0;
	double phi_measure = 0;
	// exit(1);
	for (std::map<int,double>::iterator it=ThetaC.begin(); it!=ThetaC.end(); ++it)
	{
		phi_measure = 0;
		total = 0;
		passed = 0;
		for(phi_measure = 0;  phi_measure < 2*pi; phi_measure += 2*pi/1000)
		{
			++total;
			
			Photon p(it->second, phi_measure);
			double &th = p.Theta;
			double &ph = p.Phi;
			r.Rotate_Photon(p.Theta, p.Phi);
			
			Simulate simPho(th, ph);
			simPho.SetStart(x,y,0);
			simPho.SetDim(l, w, h);
			simPho.GotoWall("no");
			double &x_p = simPho.coord[0];
			if((x_p == 0 ) || (x_p == l)){ break; } 
			Photon photon(th, ph);
			CheckAngel(d, photon, "no");
			simPho.Flag = photon.Flag;
			if(simPho.Flag != 1){ ++passed; }
		}
		madeit[it->second] = double(passed)/total*100;
		// cout << "percent passed = " << madeit[it->second] << "\n\n";
	}

}