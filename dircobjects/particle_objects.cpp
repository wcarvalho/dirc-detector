#include "dirc_objects.h"
#include <iostream>
#include <string>
#include <cmath>
#include <string>
#include "TVector3.h"
#include "TMath.h"

using namespace std;

void ParticleOut::PossibleMasses(){
	types.clear();
	masses[0]=.105658;
	masses[1]=.511e-3;
	masses[2]=.1396;
	masses[3]=.493667;
	masses[4]=.938;

	std::string options[5] = {"muon", "electron", "pion", "kaon", "proton"};
	for (unsigned int l = 0; l < 5; ++l){
		deftypes.push_back(options[l]);
		types.push_back(options[l]);
	}
}

double ParticleOut::CalculateBeta(double mass){
	if ( (pt) && (Eta) ){
		double ThetaBeam = 2*atan(exp(-Eta));
		double momentum = pt/sin(ThetaBeam);
	  return momentum/pow(( mass*mass + momentum*momentum ),.5);
	}
	else
		return -1.;
}

std::map<std::string, double> ParticleOut::MassMap(){
	map<string, double> massmap;
	for(unsigned int i = 0; i < deftypes.size(); ++i){
		massmap[deftypes.at(i)] = masses[i];
	}
	return std::move(massmap);
}

map<string, double> ParticleOut::EmissionAngleMap(){

	double pi = TMath::Pi();
	double ThetaBeam = 2*atan(exp(-Eta));
	double P = pt/sin(ThetaBeam);
	double thetaC_temp = 0.;
	double beta_temp = 0.;

	std::map<std::string, double> emissionanglemap;
	for (unsigned int i = 0; i < deftypes.size(); ++i){
		std::string &name = deftypes.at(i);
		beta_temp = P/pow(( masses[i]*masses[i] + P*P ),.5);
		thetaC_temp = acos(1./(1.474*beta_temp));
		if (thetaC_temp == thetaC_temp)
			emissionanglemap[name] = thetaC_temp;
	}
	return std::move(emissionanglemap);
}

double Particle::CalculatePhotonsPercm(double xlow, double xhigh, double n, double z, double alpha){
	double pi = TMath::Pi();
	double nu = (1-(1/(Beta*Beta*n*n)));
	double Constant = 2*pi*alpha*nu*nu;
	TF1 f("dNdx", "1/x/x", xlow, xhigh);
	PhotonsPercm = 1e-2*Constant*f.Integral(xlow, xhigh);
	return PhotonsPercm;
}
double Particle::CalculateMomentum() const{
	return sqrt((m*Beta)*(m*Beta)/(1-Beta*Beta));
}

double Particle::CalculateMomentum(){
	double ThetaBeam = 2*atan(exp(-Eta));
	momentum = pt/sin(ThetaBeam);
	return momentum;
}