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
	int nparticle_range[2] = {1,10000};										// range in number of particles
  int nevents = 5;																			// number of events
  int nparticles = 0;																		// number of particles
  int input = ai.random_given;													// number used for seed of TRandom3
  int maxPars = 1;																			// maximum number of particles that will be allowed to pass
  bool print = false;																		// bool for printing purposes


  string dirc_prop = "../dirc_prop.txt";
  string filename = "../../root_files/generator.root";
  if(ai.events_given){ nevents = ai.events_arg; }
  if(ai.particles_given)
  { 
  	nparticle_range[0] = ai.particles_arg[0];
		nparticle_range[1] = ai.particles_arg[1];
	}
	if(ai.filename_given) filename = ai.filename_arg;
	if(ai.dirc_properties_given) dirc_prop = ai.dirc_properties_arg;
	if(ai.maxpars_given) maxPars = ai.maxpars_arg; 
	if(ai.verbose_given) print = true;


	//--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
  Detector d;
  ParticleEvent ParEv;
  vector<Particle> &pars = ParEv.Particles;
  bool passed = false;
  int passes = 0;

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
	if (print == true )
	{
		printf("Detector Properties:\n");
		printf("\tLength(x) = %f, Width(y) = %f, Height(z) = %f\n", d.Length, d.Width, d.Height); 
		printf("\tMagnetic Field = %f and Radial Distance = %f\n\n", d.Mag_field, d.Radial_D); 
	}
  //__________________generate________________
  for (unsigned int ev = 0; ev < nevents; ev++)
  {
  	f.Int(nparticle_range[0], nparticle_range[1], nparticles);
  	pars.clear();
  	passes=0;
  	if (print == true ) cout << "Event = " << ev << " with " << nparticles << " particles\n";
  	for (unsigned int par = 0; par < nparticles; par++)
  	{
			gPar.gen(); if (input != 0) input++;

  		passed = intersect_with_dirc(d.Width, gPar.Eta, gPar.pt, 
	  				gPar.Phi_i, gPar.m , gPar.Charge, d.Radial_D,
	  				d.Mag_field, gPar.X, gPar.Y, gPar.Phi, gPar.Theta, gPar.Beta);
  		gPar.X = gPar.X + d.Length/2;
  		gPar.Y = gPar.Y + d.Width/2;

  		gPar.getEangle();

  		//			place function that determines the number of photons per cm here


  		//_____The following predicts the number of photons a particle will emit;
  		Simulate simPar(gPar.Theta, gPar.Phi);
			simPar.SetStart(gPar.X, gPar.Y,0);  simPar.SetDim(d.Length, d.Width, d.Height);
			simPar.DistancetoWalls( );  simPar.WhichWall( );
			gPar.NumberofPhotons = int((simPar.WillTravel()+1))*gPar.PhotonsPercm; 					// + 1 accounts for the photons released at the plane of intersection
			
  		if ((passed == true) && gPar.Theta < TMath::Pi()/2)
			{
				if( gPar.ConeAngle == gPar.ConeAngle )
				{
					if (print == true)
					{
						printf("\n\tParticle Eta = %f, pt = %f, Phi_i = %f\n", gPar.Eta, gPar.pt, gPar.Phi_i);
		  			printf("\t\tmass = %f, charge = %i\n", gPar.m, gPar.Charge);
		  			printf("\tProduced:\n"
		  				  		"\t\tX = ,%f, Y = %f, Phi = %f, Theta = %f\n"
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
  	tree.Fill();
		if (print == true ) cout << "\t" << maxPars << " particles passed\n";
  }

  file.Write();
	cout << "file: " << filename << endl;
  file.Close();
  

  return 0;
}
