//====================================================
//
//                GENERATOR
//
//====================================================

#include "../headers/generator.h"
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;
  if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }
	int nparticle_range[2] = {1,10000};			//change to 1-1000 photons								// range in number of particles
  int nevents = 5;																			// number of events
  int nparticles = 0;																		// number of particles
  int input = ai.random_given;
  int maxPars = 1;


  string dirc_prop = "../dirc_prop.txt";
  string filename = "../../root_files/generator.root";
  if(ai.events_given){ nevents = ai.events_arg; }
  if(ai.particles_given)
  { 
  	nparticle_range[0] = ai.particles_arg[0];
		nparticle_range[1] = ai.particles_arg[1];
	}
	if(ai.filename_given){ filename = ai.filename_arg; }
	if(ai.dirc_properties_given){ dirc_prop = ai.dirc_properties_arg; }
	// if(ai.maxpars_given){ dirc_prop = ai.maxpars_arg; }	


	//--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
  Detector d;																							// detector
  ParticleEvent ParEv;
  vector<Particle> &pars = ParEv.Particles;
  bool passed;
  int passes=0;

	//__________________setting up root file________________
	TFile file(filename.c_str(), "recreate");
  TTree tree("gen_out", "a tree of particles");
	tree.Branch("Particle Event", &ParEv);
	tree.Branch("detector", &d);

  Random f(input);																				// random number generator

	gParticle gPar(input); 
	gParticle *gPar_p = &gPar;
	Particle *Par = gPar_p;

	cout << "\nGENERATOR\n";
  //__________________generate________________
  for (unsigned int ev = 0; ev < nevents; ev++)
  {
  	f.Int(nparticle_range[0], nparticle_range[1], nparticles); 			// assign random number for number of particles "nparticles"
  	pars.clear();
  	passes=0;
  	for (unsigned int par = 0; par < nparticles; par++)
  	{
			gPar.gen(); if (input != 0){ input++; }
  		passed = intersect_with_dirc(d.Width, gPar.Eta, gPar.pt, 
	  				gPar.Phi_i, gPar.m , gPar.Charge, d.Radial_D,
	  				d.Mag_field, gPar.X, gPar.Y, gPar.Phi, 
	  				gPar.Theta, gPar.Beta);
  		gPar.X = gPar.X + d.Length/2;
  		gPar.Y = gPar.Y + d.Width/2;
  		// cout << "x,y = " << gPar.X<< ", " << gPar.Y << endl;
  		if (passed == 1)
			{
				if( gPar.ConeAngle == gPar.ConeAngle )
				{
				pars.push_back(*Par);
				// cout << "Eta = " << gPar.Eta << ", pt = " << gPar.pt << endl;
				passes++;
				// cout << "passed " << passes << "times\n";
				if (passes == maxPars){ break; }
				// cout << "ConeAngle = " << gPar.ConeAngle << endl;
				}
  		}
  	}
  	tree.Fill();
  }

  file.Write();
  cout << "file: " << filename << endl;
  file.Close();
  return 0;

}
