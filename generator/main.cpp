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
  double pi = TMath::Pi();
  int input = ai.random_given;													// number used for seed of TRandom3
  bool print = ai.verbose_given;																		// bool for printing purposes
  bool replace = false;
  int nevents = 5;															// number of events
  string directory = "";

  string dirc_prop = "../dirc_prop.txt";
	if(ai.dirc_properties_given) 
		dirc_prop = ai.dirc_properties_arg;
  
  string filename = "generator.root";
	if(ai.filename_given) 
		filename = ai.filename_arg;
  
  if(ai.Directory_given){
  	directory = ai.Directory_arg;
  	filename = directory.append(filename);
  }

  if(ai.events_given){ nevents = ai.events_arg; }
  
  
  int nparticles = 0;														// number of particles
	//______________ default parameters ___________________

  int maxPars_default = 100;														// maximum number of particles that will be allowed to pass
	int nparticle_range_default[2] = {1,10000};						// range in number of particles
  if(ai.particles_given){ 
  	nparticle_range_default[0] = ai.particles_arg[0];
		nparticle_range_default[1] = ai.particles_arg[1];
	}
	double etarange_default[2] = {-.5, .5};
	double ptrange_default[2] = {.2,10.};
	double phirange_default[2] = {0.,2*pi};
	double charge_default = 0;
	ParticleOut tempP;								// to extract default particle types
	vector<string> types_default = tempP.types;
	
	if(ai.maxpars_given) 
		maxPars_default = ai.maxpars_arg; 
  if(ai.particles_given)
  	ResetIntArrayParameter(ai.particles_arg, nparticle_range_default);
	//______________ parameters used in program ___________________
  int maxPars = maxPars_default;
	int nparticle_range[2]; ResetIntArrayParameter(nparticle_range_default, nparticle_range);
	double etarange[2]; ResetDoubleArrayParameter(etarange_default, etarange);
	double ptrange[2]; ResetDoubleArrayParameter(ptrange_default, ptrange);
	double phirange[2]; ResetDoubleArrayParameter(phirange_default, phirange);
	double charge = charge_default;
	vector<string> types = tempP.types;

	//--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------
  Detector d;
  ParticleEvent ParEv;
  vector<Particle> &pars = ParEv.Particles;
  vector<Particle> pars2;
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
	Particle *Par = gPar_p;											// inheritting properties of parent class for use with older libraries

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
  	cout << "Event " << ev << endl;
  	f.Int(nparticle_range[0], nparticle_range[1], nparticles);
  	pars.clear(); pars2.clear();
  	passes=0;
  	
	if(ai.custom_set_given){
		TakeInParameters(ai.custom_set_arg, nevents, maxPars, nparticle_range, etarange, ptrange, phirange, charge, types, replace);
		maxPars = maxPars_default;
		ResetIntArrayParameter(nparticle_range_default, nparticle_range);
		ResetDoubleArrayParameter(etarange_default, etarange);
		ResetDoubleArrayParameter(ptrange_default, ptrange);
		ResetDoubleArrayParameter(phirange_default, phirange);
		charge = charge_default;
		types = tempP.types;
	}

	if (!replace)
	  	pars = generate(nparticles, gPar, d, maxPars, print);
  	
	if(ai.custom_set_given){
		TakeInParameters(ai.custom_set_arg, nevents, maxPars, nparticle_range, etarange, ptrange, phirange, charge, types, replace);
		SetParameterOptions(gPar, etarange, ptrange, phirange, charge, types);
		pars2 = generate(nparticles, gPar, d, maxPars, print);

		maxPars = maxPars_default;
		ResetIntArrayParameter(nparticle_range_default, nparticle_range);
		ResetDoubleArrayParameter(etarange_default, etarange);
		ResetDoubleArrayParameter(ptrange_default, ptrange);
		ResetDoubleArrayParameter(phirange_default, phirange);
		charge = charge_default;
		types = tempP.types;
		SetParameterOptions(gPar, etarange, ptrange, phirange, charge, types);
		if (!replace){
			for (unsigned int i = 0; i<pars2.size(); ++i)
				pars.push_back(pars2.at(i));
		}
		else{
			pars = pars2;
		}
	}
	cout << "\ttotal particles: " << pars.size() << endl;
   	tree.Fill();

  }

  file.Write();
	cout << "file: " << filename << endl;
  file.Close();
  

  return 0;
}
//---------------------------------------------------------------
void SetParameterOptions(gParticle& gPar, double etarange[2], double ptrange[2], double phirange[2], double charge, vector<string> types){

	gPar.SetEtaRange(etarange[0], etarange[1]);
	gPar.SetPhiRange(phirange[0], phirange[1]);
	gPar.SetPtRange(ptrange[0], ptrange[1]);
	gPar.SetTypes(types);
	gPar.SetChargeMarker(charge);
}

void ResetDoubleArrayParameter(double Default[2], double Used[2]){
	for (unsigned int i = 0; i<2; ++i)
		Used[i] = Default[i];
}

void ResetIntArrayParameter(int Default[2], int Used[2]){
	for (unsigned int i = 0; i<2; ++i)
		Used[i] = Default[i];
}