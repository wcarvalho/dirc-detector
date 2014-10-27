#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifndef ran1
#define ran1

//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include "stdlib.h"

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"
#include "TF1.h"


//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
#include "Simulate.h"

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

//class for generating particles
class gParticle : public Particle
{
public:
	gParticle(double in = 0) : r(in), chargeMarker(0) { setDefaults(); }
	~gParticle(){}

	void setDefaults();
	void genEta(){ Eta = r.Uniform(etarange[0],etarange[1]); }
	void genMass();
	void genPT();
	void genCharge();
	void getPhi(){ Phi_i = r.Uniform(phirange[0],phirange[1]); }
	void getEangle(){	ConeAngle = acos(1./(1.474*Beta));}
	void gen(){ genEta(); genMass(); genPT(); genCharge(); getPhi(); }

	void SetPhiRange(double low, double hi){ phirange[0] = low; phirange[1] = hi; }	

	void SetEtaRange(double low, double hi){ etarange[0] = low; etarange[1] = hi; }

	void SetPtRange(double low, double hi){ ptrange[0] = low; ptrange[1] = hi; }
	void SetTypes(vector<string> newTypes){ types = newTypes; }
	void SetChargeMarker(double marker){ chargeMarker = marker; }

	double Phi_i;
	Random r;
	double etarange[2];
	double phirange[2];
	double ptrange[2];
	map<string, double> massmap;
	double chargeMarker;
};

vector<Particle> generate(int nparticles, gParticle& gPar, Detector d, int maxPars, bool print);

bool intersect_with_dirc(double dirc_height, double eta, double pt, double phi_input, double mass, int charge, double radius, double magfield, double& x_dirc, double& y_dirc, double& phi_dirc, double& theta_dirc, double& beta);

void TakeInParameters(string file, int& nevents, int& maxPars, int nparticle_range[2], double etarange[2], double ptrange[2], double phirange[2], double charge, vector<string> &types, bool& replace);

void SetParameterOptions(gParticle& gPar, double etarange[2], double ptrange[2], double phirange[2], double charge, vector<string> types);
void ResetDoubleArrayParameter(double Default[2], double Used[2]);
void ResetIntArrayParameter(int Default[2], int Used[2]);

#endif