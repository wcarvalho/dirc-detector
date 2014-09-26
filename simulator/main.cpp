//====================================================
//
//                SIMULATOR
//
//====================================================

#include "../headers/simulator.h"
#include <sstream>
#include <cstdlib>
#include "cmdline.h"

int main(int argc, char** argv)
{
	gengetopt_args_info ai;  
	if (cmdline_parser (argc, argv, &ai) != 0){ exit(1); }
	if (ai.new_given)
	{
	  system("exec ./../../generator/build/generator");
	}
	int input = ai.random_given;
	string readf = "../../root_files/generator.root";
	string writef = "../../root_files/simulator.root";
	

	//_______Declarations__________
	bool print = false;
	Detector *d = 0;
	ParticleEvent *ParEvent = 0;
	PhotonEvent photon_event; photon_event.Photons.clear();
	GeneratorOut *event_output = 0;
  
  Displayer Output;
	Rotater r;

	if(ai.verbose_given) print = true;
	if(ai.readfile_given) readf = ai.readfile_arg;
	if(ai.writefile_given) writef = ai.writefile_arg;
	//________read datafile______________
  TFile rf(readf.c_str(), "read");
  TTree* gen_out = (TTree*)rf.Get("gen_out");
	gen_out->SetBranchAddress("Particle Event", &ParEvent);
	gen_out->SetBranchAddress("detector", &d);

	//________write datafile______________
	TFile wf(writef.c_str(), "recreate");
	TTree sim_out("sim_out", "simulation output");
	sim_out.Branch("simEvent", &event_output);

  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------

	cout << "\nSIMULATOR\n";
	//___for convenience__________________________
	vector<Particle> *pars = &ParEvent->Particles;
	double *par_theta, *par_phi;
	double *pho_theta, *pho_phi;
	//____________________________________________
	
	d->get_Critical_Angle(1.);
	for (int ev = 0; ev < gen_out->GetEntries(); ev++)
	{
		if (print == true)
		{
			printf("\tevent %i\n", ev);
		}
		gen_out->GetEntry(ev);
		photon_event.Photons.clear();
		photon_event.iterator = 0;

		for (unsigned int par = 0; par < ParEvent->Particles.size(); par++)
		{
			par_theta = &ParEvent->Particles[par].Theta;
			par_phi = &ParEvent->Particles[par].Phi;
			if (print){
				printf("\t\tparticle theta = %f, phi = %f\n", *par_theta, *par_phi);
			}
			Simulate_ParticlePath(*d, ParEvent->Particles[par], photon_event, 1, print);
			r.Feed_Particle(*par_theta, *par_phi);
			for(int &pho = photon_event.iterator; pho < photon_event.Photons.size(); pho++)
			{

				pho_theta = &photon_event.Photons[pho].Theta;
				pho_phi = &photon_event.Photons[pho].Phi;
				r.Rotate_Photon(*pho_theta, *pho_phi);
				photon_event.Photons[pho].UnitVector = Get_UnitVector(*pho_theta, *pho_phi);
			}
		}

		double totalphotons = photon_event.Photons.size();

		for (int i = 0; i < photon_event.Photons.size(); i++)
		{
			Simulate_PhotonPath(*d, photon_event.Photons[i]);
			CheckForFlag(photon_event, i, Output.Trivial);
		}
		if (print)
		{
			printf("\t\t%i passed (%f percent)\n\n", int(photon_event.Photons.size()), photon_event.Photons.size()/totalphotons*100.);
		}
		FillTree(sim_out, *ParEvent, photon_event, *event_output, Output.Important);
	}
  
  wf.Write();
  wf.Close();
  rf.Close();
  cout << "file: " << writef << endl;
  
  return 0;
}