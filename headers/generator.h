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

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

class gParticle : public Particle
{
public:
	gParticle(double in = 0) : r(in) 
		{ masses[0]=(.105658); masses[1]=(.511); masses[2]=(.1396);
			masses[3]=(.493667); masses[4]=(.938); gen(); }
	~gParticle(){}

	void genEta(){ Eta = r.Uniform(-.5,.5); }
	void genMass();
	void genPT();
	void genCharge();
	void getEangle(double n = 1.474){	ConeAngle = acos(1/(n*Beta));}
	void getPhi(){ Phi_i = r.Uniform(0,2*TMath::Pi()); }
	void gen(){ genEta(); genMass(); genPT(); getEangle(); genCharge(); getPhi(); }

	double masses[5];
	double Phi_i;
	Random r;
};

// push_particle(sevent.sPar.back(), sParticle);
bool intersect_with_dirc(double dirc_height, double eta, double pt, double phi_input, double mass, int charge, double radius, double magfield, double& x_dirc, double& y_dirc, double& phi_dirc, double& theta_dirc, double& beta);

#endif