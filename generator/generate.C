#include "dirc_objects.h"
#include "../headers/generator.h"
#include <vector>

vector<Particle> generate(int nparticles, gParticle& gPar, Detector d, int maxPars, bool print){

	vector<Particle> pars; pars.clear();
	bool passed = false;
	gParticle *gPar_p = &gPar;
	Particle *Par = gPar_p;
	int passes = 0;

	for (unsigned int par = 0; par < nparticles; par++)
  	{
  		if (maxPars == 0) break;
			gPar.gen();

  		passed = intersect_with_dirc(d.Width, gPar.Eta, gPar.pt, 
	  				gPar.Phi_i, gPar.m , gPar.Charge, d.Radial_D,
	  				d.Mag_field, gPar.X, gPar.Y, gPar.Phi, gPar.Theta, gPar.Beta);
  		gPar.X = gPar.X + d.Length/2;
  		gPar.Y = gPar.Y + d.Width/2;

  		gPar.getEangle();

  		//			place function that determines the number of photons per cm here


  		//_____The following predicts the number of photons a particle will emit;
  		Simulate simPar(gPar.Theta, gPar.Phi);
			simPar.SetStart(gPar.X, gPar.Y,0);
			simPar.SetDim(d.Length, d.Width, d.Height);
			simPar.DistancetoWalls( );  simPar.WhichWall( );
			gPar.NumberofPhotons = int((simPar.WillTravel()+1))*gPar.PhotonsPercm; 					// + 1 accounts for the photons released at the plane of intersection
			
  		if (passed && (gPar.Theta < TMath::Pi()/2))
			{
				if( gPar.ConeAngle == gPar.ConeAngle )
				{
					if (print)
					{
						printf("\n\tParticle: Eta = %f, pt = %f, Phi_i = %f\n", gPar.Eta, gPar.pt, gPar.Phi_i);
		  			printf("\t\tmass = %f, charge = %i\n", gPar.m, gPar.Charge);
		  			printf("\tCollided with detector with:\n"
		  				  		"\t\tX = %f, Y = %f, Phi = %f, Theta = %f\n"
		  				  		"\t\tBeta = %f, Emission Angle = %f\n"
		  				  		"\t\twith %i expected photons\n",
		  				  		gPar.X, gPar.Y, gPar.Phi, gPar.Theta, gPar.Beta, gPar.ConeAngle, gPar.NumberofPhotons);
		  		}
				pars.push_back(*Par);
				passes++;

				if (passes == maxPars){ break; }
				}
  		}
  	}

  	if (print) cout << "\t" << passes << " particles passed\n";
  	return pars;
}