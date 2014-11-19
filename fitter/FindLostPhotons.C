#include "iostream"
#include <vector>
#include <cmath>
#include <string>
#include "TObject.h"
#include "../headers/fitter.h"
#include "../headers/simulator.h"
 
using namespace std;

void RiemannSum(double x, double y, double stepsize){
	
}

void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &madeit, double &traveled, bool print){
	
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
	double phi_measure = 0;

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
		simPar.Traveled = 0;
		while(simPar.Traveled < Path_length){
			for(phi_measure = 0;  phi_measure < 2*pi; phi_measure += 2*pi/PhiSteps)
			{
				++total;
				Photon p(it->first, phi_measure);
				double &th = p.Theta;
				double &ph = p.Phi;
				r.Rotate_Photon(th, ph);
				
				Simulate simPho(th, ph);
				simPho.SetStart(simPar.coord[0],simPar.coord[1],simPar.coord[2]);
				simPho.SetDim(l, w, h);
				bool cut = false;
				for(unsigned int reflections = 0; reflections< 4; ++reflections){
					// cout << "\treflections = " << reflections << endl;
					simPho.GotoWall("no");
					double &x_p = simPho.coord[0];
					if((x_p == 0 ) || (x_p == l)){ break; } 
					Photon photon(th, ph);
					photon.Wall = simPho.wall;
					d.get_Critical_Angle(1.);
					CheckAngel(d, photon, "no");
					if(photon.Flag){
						cut = true;
						// cout << "\t\tcut\n";
					}
				}
				if (!cut) ++passed;
			}
			simPar.TravelDistance(Path_length/PathSteps);
		}
		madeit[it->first] = double(passed)/total;
		// cout << "percent = " << double(passed)/total << endl;
	}

}