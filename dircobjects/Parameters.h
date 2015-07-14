#ifndef Parameters
#define Parameters
//
//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TGraph.h"



//------------------------------------------
//		mine
//------------------------------------------
// #include "Rotater.h"

using namespace std;		

class PhotonParameters
{
public:
	PhotonParameters() {}
	~PhotonParameters(){}

	void Range_Phi(double low, double hi){Phi[0] = low; Phi[1] = hi;}

	double Phi[2];

};

class ParticleParameters
{
public:
	ParticleParameters() {}
	~ParticleParameters(){}

	void Range_Theta(double low = 0., double hi = 1.0){Theta[0] = low; Theta[1] = hi;}
	void Range_Phi(double low = 0., double hi = 2*TMath::Pi()){Phi[0] = low; Phi[1] = hi;}
	void Range_X(double low, double hi){X[0] = low; X[1] = hi;}
	void Range_Y(double low, double hi){Y[0] = low; Y[1] = hi;}
	void Range_Cone(double low = .7, double hi = .9){Cone[0] = low; Cone[1] = hi;}
	void Range_Photons(int low = 400, int hi = 1000){Photons[0] = low; Photons[1] = hi;}
	void Range_Emissions(int low = 0, int hi = 5){Emissions[0] = low; Emissions[1] = hi;}

	double Theta[2];
	double Phi[2];
	double X[2];
	double Y[2];
	double Cone[2];
	double Photons[2];
	double Emissions[2];
};

class Parameter
{
public:
	Parameter() {}
	~Parameter(){}

	void Range_Particles(int low = 1, int hi = 1){Particles[0] = low; Particles[1] = hi;}

	PhotonParameters Photon;
	ParticleParameters Particle;
	double Particles[2];

};

#endif