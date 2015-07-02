//====================================================
//
//                GENERATOR
//
//====================================================

#include "generator.h"
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;
  if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }

  double pi = TMath::Pi();

  // command line arguments

  bool print = ai.verbose_given;								// bool for printing purposes
  bool quiet = ai.quiet_given; if (quiet) print = !quiet;
  string dirc_prop = ai.dirc_properties_arg;
  string filename = ai.output_arg;
  string directory = "";
  if(ai.Directory_given){
  	directory = ai.Directory_arg;
  	filename = directory.append(filename);
  }

  int seed = ai.seed_arg;											// seed for TRandom
	int nevents = ai.events_arg;									// number of events
  int maxPars = ai.maxpars_arg;									// maximum # particles to hit DIRC
  double charge = ai.char_arg;
  bool replace;

	int nparticle_range[2] {1,1000};
	double etarange[2] {-.5, .5};
	double ptrange[2] {.2, 10.};
	double phirange[2] {0., 2*pi};
	ParticleOut Pt; vector<string> types = Pt.deftypes;

  if(ai.nparticles_given) equate_arrays(nparticle_range, ai.nparticles_arg);
  if(ai.etar_given) equate_arrays(etarange, ai.etar_arg);
  if(ai.ptr_given) equate_arrays(ptrange, ai.ptr_arg);
  if(ai.phir_given) equate_arrays(phirange, ai.phir_arg);
  if(ai.types_given) types.clear();
  for (unsigned i = 0; i < ai.types_given; ++i){
  	types.push_back(ai.types_arg[i]);
  }

	double fixed_momentum {-1.};
  if(ai.p_given) fixed_momentum = ai.p_arg;

  if (ai.ptr_given && ai.p_given){
  	cout << "Please only provide the p or pt\n"; exit(1);
  }


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

  Random f(seed);						// random number generator
	gParticle gPar(seed);
	gParticle *gPar_p = &gPar;
	Particle *Par = gPar_p;                    // inheritting properties of parent class for use with older libraries
    if (ai.pt_func_given) gPar.setPtDistributionFunction(ai.pt_func_arg);

	if (print == true )
	{
		printf("Detector Properties:\n");
		printf("\tLength(x) = %f, Width(y) = %f, Height(z) = %f\n", d.Length, d.Width, d.Height);
		printf("\tMagnetic Field = %f and Radial Distance = %f\n\n", d.Mag_field, d.Radial_D);
	}

  //__________________generate________________
  for (unsigned int ev = 0; ev < nevents; ev++)
  {
  	if(!quiet) cout << "Event " << ev << endl;
		static int nparticles;														// number of particles
  	f.Int(nparticle_range[0], nparticle_range[1], nparticles);
  	pars.clear();
  	passes=0;

	if(ai.custom_set_given){
		TakeInParameters(ai.custom_set_arg, nevents, maxPars, nparticle_range, etarange, ptrange, phirange, charge, types, replace);
	}
	SetParameterOptions(gPar, etarange, ptrange, phirange, charge, types);
	pars = generate(nparticles, gPar, d, maxPars, fixed_momentum, print);

	if(!quiet) cout << "\ttotal particles: " << pars.size() << endl;
   	tree.Fill();
  }

  file.Write();
	if(!quiet) cout << "file: " << filename << endl;
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

template < typename T >
void equate_arrays(T array1[], T array2[], int size){
	for (unsigned i = 0; i < size; ++i)
		array1[i] = array2[i];
}

void ResetDoubleArrayParameter(double Default[2], double Used[2]){
	for (unsigned int i = 0; i<2; ++i)
		Used[i] = Default[i];
}

void ResetIntArrayParameter(int Default[2], int Used[2]){
	for (unsigned int i = 0; i<2; ++i)
		Used[i] = Default[i];
}