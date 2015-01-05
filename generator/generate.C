#include "dirc_objects.h"
#include "../headers/generator.h"

vector<Particle> generate(int nparticles, gParticle& gPar, Detector d, int maxPars, bool print){

	vector<Particle> pars; pars.clear();
	bool passed = false;
	gParticle *gPar_p = &gPar;
	Particle *Par = gPar_p;
	int passes = 0;
	double pi = TMath::Pi();

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

  		// determines the number of photons per cm here
  		double xlow = 200e-9;
  		double xhigh = 1000e-9;
  		double z = 1.;
  		double alpha = 1./137;
  		double n = d.n;
  		double beta = gPar.Beta;
  		double nu = (1-(1/(beta*beta*n*n)));
  		double Constant = 2*pi*alpha*nu*nu;
  		TF1 f("dNdx", "1/x/x", xlow, xhigh);
  		
  		double dNdx = 1e-2*Constant*f.Integral(xlow, xhigh);
  		gPar.PhotonsPercm = dNdx;
  		if (passed && (gPar.Theta < pi/2))
			{
				if( gPar.ConeAngle == gPar.ConeAngle )
				{
					if (print)
					{
						printf("\n\tParticle: %s, Eta = %f, pt = %f, Phi_i = %f\n", gPar.name.c_str(), gPar.Eta, gPar.pt, gPar.Phi_i);
		  			printf("\t\tmass = %f, charge = %i\n", gPar.m, gPar.Charge);
		  			printf("\tCollided with detector with:\n"
		  				  		"\t\tX = %f, Y = %f, Phi = %f, Theta = %f\n"
		  				  		"\t\tBeta = %f, Emission Angle = %f\n"
		  				  		"\t\twith %i PhotonsPercm\n",
		  				  		gPar.X, gPar.Y, gPar.Phi, gPar.Theta, gPar.Beta, gPar.ConeAngle, gPar.PhotonsPercm);
		  		}
				pars.push_back(*Par);
				passes++;

				if (passes == maxPars){ break; }
				}
  		}
  	}

  	// cout << "\t" << pars.size() << " particles passed\n";
  	return pars;
}