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
	int input = ai.random_given;

	string readf = ai.input_arg;
	FileProperties readf_prop(readf);
	string directory = "";

	string writef = "simulator.root";
	string writef2 = "cheat.root";
	string status = "recreate";
	string non_cheatCopyFile = "";
	string cheatCopyFile = "";
	double smear = .00;

	//_______Declarations__________
	bool print = ai.verbose_given;
	bool Append = ai.Append_given;
  bool quiet = ai.quiet_given; if (quiet) print = !quiet;
	bool write = !ai.file_write_off_given;
	Detector *d = 0;
	ParticleEvent *ParEvent = 0;
	PhotonEvent photon_event;
	GeneratorOut *event_output = 0;
	GeneratorOut *event_outputCopy = 0;

  Displayer Output;
	Rotater r;
	FileProperties &f = readf_prop;

	if(ai.writefile_given) writef = ai.writefile_arg;
	if(ai.cheatfile_given) writef2 = ai.cheatfile_arg;

	if(ai.Directory_given) {
		string temp_dir = ai.Directory_arg;
		if (temp_dir.size() != 0)	directory = ai.Directory_arg;
		else directory = readf_prop.directory;
	}

	f.appendFileToDirectory(directory, writef);
	FileProperties writef_prop(writef);
	f.appendFileToDirectory(directory, writef2);


	if(ai.smear_given) smear = ai.smear_arg;

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
	sim_out.Branch("detector", &d);

	TFile wf2(writef2.c_str(), status.c_str());
	TTree cheat_info("cheat_info", "all information for event");
	cheat_info.Branch("Particle Event", &ParEvent);
	cheat_info.Branch("Photon Event", &photon_event);
	cheat_info.Branch("detector", &d);



  //--------------------------------------------------
  //              Beginning of Program;
  //--------------------------------------------------

	double pi = TMath::Pi();
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

	for (int ev = 0; ev < gen_out->GetEntries(); ev++)
	{
		if(!quiet) printf("Event %i\n", ev);
		gen_out->GetEntry(ev);

		if (Append)	{
			non_cheatCopyTree->GetEntry(ev);
			cheatCopyTree->GetEntry(ev);
		}

		cheat_info.GetEntry(ev);
		photon_event.Photons.clear();
		photon_event.iterator = 0;

		d->get_Critical_Angle(1.);
		if (print) cout << "Number of Particles: " << ParEvent->Particles.size() << endl;
		for (unsigned int par = 0; par < ParEvent->Particles.size(); par++)
		{
 			par_theta = &ParEvent->Particles[par].Theta;
			par_phi = &ParEvent->Particles[par].Phi;

			if (print) cout << "\tParticle = " << par << " with X, Y, Theta, Phi, Beta =\n\t\t" << ParEvent->Particles[par].X << ", " << ParEvent->Particles[par].Y << ", " << ParEvent->Particles[par].Theta << ", " << ParEvent->Particles[par].Phi << ", " << ParEvent->Particles[par].Beta << endl;
			if (print) cout << "\t\tPhotonsPerCm = " << ParEvent->Particles.back().PhotonsPercm << endl;
			// if (print){
				// printf("\t\tparticle theta = %f, phi = %f\n", *par_theta, *par_phi);
			// }
			Simulate_ParticlePath(*d, ParEvent->Particles[par], par, photon_event, 1, print);
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
			Simulate_PhotonPath(*d, photon_event.Photons[i], smear, print);
			if (photon_event.Photons[i].Flag == 1){
				ParEvent->Particles.at(photon_event.Photons[i].WhichParticle).nPhotonsPassed -= 1;
			}
			CheckForFlag(photon_event, i, Output.Trivial);
		}
		if (ai.print_photons_given) cout << "total photons: " << photon_event.Photons.size() << endl;
		FillTree(sim_out, *ParEvent, photon_event, *event_output, Output.Important, event_outputCopy, Append, quiet);
		if (print){
			printf("\t\t(%f percent)\n\n", photon_event.Photons.size()/totalphotons*100.);
		}
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
  if(!quiet) cout << "file: " << writef << endl;

  return 0;
}