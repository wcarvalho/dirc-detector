#include "dirc_objects.h"
#include "generator.h"

double calculate_pt_from_momentum(double const& eta, double const& momentum);
vector<Particle> generate(int nparticles, gParticle& gPar, Detector d, int maxPars, double const& fixed_momentum, bool print){

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
			if (fixed_momentum >= 0) gPar.pt = calculate_pt_from_momentum(gPar.Eta, fixed_momentum);

			gPar.CalculateMomentum();

  		passed = intersect_with_dirc(d.Width, gPar.Eta, gPar.pt,
	  				gPar.Phi_i, gPar.m , gPar.Charge, d.Radial_D,
	  				d.Mag_field, gPar.X, gPar.Y, gPar.Phi, gPar.Theta, gPar.Beta, gPar.arc_traveled_to_dirc);
  		gPar.X = gPar.X + d.Length/2;
  		gPar.Y = gPar.Y + d.Width/2;
  		gPar.Time_Traveled = (100*gPar.path_to_dirc())/(gPar.Beta*3);
  		gPar.getEangle();
  		// determines the number of photons per cm here
			gPar.CalculatePhotonsPercm(200e-9, 1000e-9, d.n);
  		if (passed)
			{
	  		// cout << "Particle Time_Traveled = " << gPar.Time_Traveled << endl;
				if( gPar.ConeAngle == gPar.ConeAngle )
				{
					if (print)
					{
						printf("\n\tParticle: %s, Eta = %f, pt = %f, p = %f, Phi_i = %f\n", gPar.name.c_str(), gPar.Eta, gPar.pt, gPar.momentum, gPar.Phi_i);
		  			printf("\t\tmass = %f, charge = %i\n", gPar.m, gPar.Charge);
		  			printf("\tCollided with detector with:\n"
		  				  		"\t\tX = %f, Y = %f, Z = %f"
		  				  		"\n\t\tPhi = %f, Theta = %f\n"
		  				  		"\t\tBeta = %f, Emission Angle = %f\n"
		  				  		"\t\twith %f PhotonsPercm, and distance to dirc %f \n",
		  				  		gPar.X, gPar.Y, gPar.Z, gPar.Phi, gPar.Theta, gPar.Beta, gPar.ConeAngle, gPar.PhotonsPercm, gPar.path_to_dirc());
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


double calculate_pt_from_momentum(double const& eta, double const& momentum){
	static double ThetaBeam; ThetaBeam = 2*atan(exp(-eta));
	double pt = momentum*sin(ThetaBeam);
	return std::move(pt);
}