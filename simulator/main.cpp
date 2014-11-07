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
	string writef2 = "../../root_files/cheat.root";
	string inputs = "inputs.txt";
	string status = "recreate";
	string non_cheatCopyFile = "";
	string cheatCopyFile = "";
	

	//_______Declarations__________
	bool print = ai.verbose_given;
	bool Append = ai.Append_given;

	Detector *d = 0;
	ParticleEvent *ParEvent = 0;
	PhotonEvent photon_event; photon_event.Photons.clear();
	GeneratorOut *event_output = 0;
	GeneratorOut *event_outputCopy = 0;
  
  Displayer Output;
	Rotater r;

	if(ai.readfile_given) readf = ai.readfile_arg;
	if(ai.writefile_given) writef = ai.writefile_arg;

	string prefix = returnBeg(readf, returnEnd(readf, "/"));
	string tempfile = returnEnd(readf, "/");				// get file name for input
	string inputpath = prefix; inputpath.append(inputs);
	addtoFile(inputpath, tempfile);
	if (Append){
		non_cheatCopyFile = copyFile(writef, ".root");
		cheatCopyFile = copyFile(writef2, ".root");
		cout << "non_cheatCopyFile = " << non_cheatCopyFile << endl;
		cout << "cheatCopyFile = " << cheatCopyFile << endl;
	}

	TFile* non_cheatCopy = 0;
	TFile* cheatCopy = 0;
	
	TTree* non_cheatCopyTree = 0;
	TTree* cheatCopyTree = 0;
	
	ParticleEvent* ParEventCopy = 0;

	if (Append){
		non_cheatCopy = TFile::Open(non_cheatCopyFile.c_str(), "read");
		non_cheatCopyTree = (TTree*)non_cheatCopy->Get("sim_out");
		// non_cheatCopyTree->Print();
		non_cheatCopyTree->SetBranchAddress("simEvent", &event_outputCopy);

		cheatCopy = TFile::Open(cheatCopyFile.c_str(), "read");
		cheatCopyTree = (TTree*)cheatCopy->Get("cheat_info");
		cheatCopyTree->SetBranchAddress("Particle Event", &ParEventCopy);
		cheatCopyTree->SetBranchAddress("simEvent", &event_outputCopy);
		}
	//________read datafile______________
  TFile rf(readf.c_str(), "read");
  TTree* gen_out = (TTree*)rf.Get("gen_out");
	gen_out->SetBranchAddress("Particle Event", &ParEvent);
	gen_out->SetBranchAddress("detector", &d);

	//________write datafile______________
	TFile wf(writef.c_str(), status.c_str());
	TTree sim_out("sim_out", "simulation output");
	sim_out.Branch("simEvent", &event_output);

	TFile wf2(writef2.c_str(), status.c_str());
	TTree cheat_info("cheat_info", "all information for event");
	cheat_info.Branch("Particle Event", &ParEvent);
	cheat_info.Branch("simEvent", &event_output);



  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------



	cout << "\nSIMULATOR\n";
	//___for convenience__________________________
	vector<Particle> *pars = &ParEvent->Particles;
	double *par_theta, *par_phi;
	double *pho_theta, *pho_phi;
	//____________________________________________
	
	// merge old and new ParticleEvent
	if (Append){
		for (unsigned int i = 0; i < pars->size(); ++i)
			ParEventCopy->Particles.push_back(pars->at(i));
	ParEvent->Particles = ParEventCopy->Particles;
	}
	//------------------------------------------------

	d->get_Critical_Angle(1.);
	for (int ev = 0; ev < gen_out->GetEntries(); ev++)
	{
		// if (print == true)
		// {
			printf("event %i\n", ev);
		// }
		gen_out->GetEntry(ev);
		
		if (Append)	{
			non_cheatCopyTree->GetEntry(ev);
			cheatCopyTree->GetEntry(ev);
		}

		cheat_info.GetEntry(ev);
		photon_event.Photons.clear();
		photon_event.iterator = 0;

		for (unsigned int par = 0; par < ParEvent->Particles.size(); par++)
		{
			par_theta = &ParEvent->Particles[par].Theta;
			par_phi = &ParEvent->Particles[par].Phi;
			// if (print){
				// printf("\t\tparticle theta = %f, phi = %f\n", *par_theta, *par_phi);
			// }
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
		
		FillTree(sim_out, *ParEvent, photon_event, *event_output, Output.Important, event_outputCopy, Append);
		cheat_info.Fill();
	}
  
  if (Append){
  	non_cheatCopy->cd();
	  non_cheatCopy->Close();

	  cheatCopy->cd();
	  cheatCopy->Close();
	}

  wf.cd();
  wf.Write();
  wf.Close();

  wf2.cd();
  wf2.Write();
  wf2.Close();

  rf.cd();
  rf.Close();
  cout << "file: " << writef << endl;
  
  return 0;
}