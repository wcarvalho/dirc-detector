#include "dirc_objects.h"
#include "../headers/generator.h"

//						generate_sPar Class
//=============================================================================================
void gParticle::genMass()
{
	int i;
	r.Int(0,4,i);
	m = masses[i];
}

void gParticle::genPT()
{
	TF1 f1("momentum", "x/(0.05+x*x*x)", 0.2, 10.);
  pt = f1.GetRandom();
}

void gParticle::genCharge()
{
	Charge = 1;
	int i;
	r.Int(0,2,i);
	if (i == 0){ Charge = -1; }
}