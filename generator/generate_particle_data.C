#include "dirc_objects.h"
#include "../headers/generator.h"

//						generate_sPar Class
//=============================================================================================
void gParticle::setDefaults(){
	//makes mass map
	for (unsigned int l = 0; l < 5; ++l){
		massmap[types[l]] = masses[l];
	}

	// sets default parameters for particle generation
	etarange[0] = -.5;
	etarange[1] = .5;
	phirange[0] = 0;
	phirange[1] = 2*TMath::Pi();
	ptrange[0] = .2;
	ptrange[1] = 10.;
}


void gParticle::genMass()
{
	int i = 0;
	r.Int(0,types.size(),i);
	m = massmap[types[i]];
	name = types[i];
}

void gParticle::genPT()
{
	TF1 f1("momentum", "x/(0.5+x*x*x*x)", ptrange[0], ptrange[1]);
  pt = f1.GetRandom();
}

void gParticle::genCharge()
{
	if (chargeMarker < 0) Charge = -1;
	if (chargeMarker > 0) Charge = 1;
	if (chargeMarker == 0){
		Charge = 1;
		int i;
		r.Int(0,2,i);
		if (i == 0) Charge = -1;
	}
}