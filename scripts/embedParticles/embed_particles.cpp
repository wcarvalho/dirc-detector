#include "dirc_objects.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// #include <sys/time.h>

#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
#include <tclap/CmdLine.h>
#include "simulator.h"
#include "Rotater.h"

using namespace std;
int main(int argc, char* argv[]){

string embedfile;
string photonsin;
string cheatin;

string photonsout;
string cheatout;

int seed;

bool print;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> embedFileArg("e","embed-file","file with particles to embed",true,"embed.root","string", cmd);

	TCLAP::ValueArg<std::string> photoninFileArg("p","photon-in-file","file with results from simulation data (photon data)",true,"photons.root","string", cmd);
	TCLAP::ValueArg<std::string> photonoutFileArg("P","photon-out-file","file with results from simulation data (photon data)",true,"photons.root","string", cmd);

	TCLAP::ValueArg<std::string> cheatinFileArg("c","cheat-in-file","file with cheat data from simulation",false,"cheat.root","string", cmd);
	TCLAP::ValueArg<std::string> cheatoutFileArg("C","cheat-out-file","file with cheat data from simulation",false,"cheat.root","string", cmd);

	TCLAP::ValueArg<int> seedArg("s","seed","the value with each the angle will be seeded",false,0.01,"string", cmd);


	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv);
	embedfile = embedFileArg.getValue();
	photonsin = photoninFileArg.getValue();
	photonsout = photonoutFileArg.getValue();

	cheatin = cheatinFileArg.getValue();
	cheatout = cheatoutFileArg.getValue();
	seed = seedArg.getValue();
	print = verboseArg.getValue();
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	GeneratorOut *old_event_output = 0;
	PhotonEvent *old_photon_event = 0;
	ParticleEvent *old_ParEvent = 0;

	ParticleEvent *ParEvent = 0;
	Detector *d = 0;


	// To Embed
	TFile embedTFile(embedfile.c_str(), "read");
	TTree* embedTree = (TTree*)embedTFile.Get("gen_out");
	embedTree->SetBranchAddress("Particle Event", &ParEvent);


	TFile* fin2 = 0;
	TTree* inputTree1 = 0;
	TTree* inputTree2 = 0;
	// Old Data
	TFile fin1(photonsin.c_str(), "read");
	inputTree1 = (TTree*)fin1.Get("sim_out");
	inputTree1->SetBranchAddress("simEvent", &old_event_output);
	inputTree1->SetBranchAddress("detector", &d);

	fin2 = TFile::Open(cheatin.c_str(), "read");
 	inputTree2 = (TTree*)fin2->Get("cheat_info");
 	inputTree2->SetBranchAddress("Particle Event", &old_ParEvent);
	inputTree2->SetBranchAddress("Photon Event", &old_photon_event);
	inputTree2->SetBranchAddress("detector", &d);


	TFile* fout2 = 0;
	TTree* outputTree1 = 0;
	TTree* outputTree2 = 0;
	// Data with embedding
	TFile fout1(photonsout.c_str(), "recreate");
	outputTree1 = inputTree1->CloneTree(0);

	fout2 = TFile::Open(cheatout.c_str(), "recreate");
	outputTree2 = inputTree2->CloneTree(0);

	vector<Photon>* Photons = 0;
	vector<Particle> *pars = &ParEvent->Particles;
	double *par_theta = 0, *par_phi = 0;
	double *pho_theta = 0, *pho_phi = 0;

	auto embed_data = [](
		PhotonEvent& new_photon_event, PhotonEvent& old_photon_event, GeneratorOut& old_generator_output, ParticleEvent& new_particle_event, ParticleEvent& old_particle_event, bool print){

		vector<Particle>& old_pars = old_particle_event.Particles;
		vector<ParticleOut>& old_parOuts = old_generator_output.Particles;
		vector<Particle>& new_pars = new_particle_event.Particles;

		vector<Photon>& old_phos = old_photon_event.Photons;
		vector<PhotonOut>& old_phoOuts = old_generator_output.Photons;
		vector<Photon>& new_phos = new_photon_event.Photons;

		if (print){
			cout << "nphotons pre:" << old_phos.size() << endl;
			cout << "nparticles pre:" << old_pars.size() << endl;
		}

		Photon* PHO = 0;
		for (auto pho: new_phos){
			PHO = &pho;
			old_phos.push_back(*PHO);
			old_phoOuts.push_back(*PHO);
		}
		Particle* PAR = 0;
		for (auto par: new_pars){
			PAR = &par;
			old_pars.push_back(*PAR);
			old_parOuts.push_back(*PAR);
		}
		if (print){
			cout << "nphotons post:" << old_phos.size() << endl;
			cout << "nparticles post:" << old_pars.size() << endl;
		}
	};
	PhotonEvent photon_event;
	for (unsigned int ev = 0; ev < inputTree1->GetEntries(); ++ev){
		embedTree->GetEntry(ev);
		inputTree1->GetEntry(ev);
		outputTree1->GetEntry(ev);
		inputTree2->GetEntry(ev);
		outputTree2->GetEntry(ev);
		// for each event, iterate over the photons

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
			Simulate_ParticlePath(*d, ParEvent->Particles[par], old_ParEvent->Particles.size()+par, photon_event, seed, print);
			static Rotater r;
			r.Feed_Particle(*par_theta, *par_phi);
			for(int &pho = photon_event.iterator; pho < photon_event.Photons.size(); pho++)
			{
				pho_theta = &photon_event.Photons[pho].Theta;
				pho_phi = &photon_event.Photons[pho].Phi;
				// if (pho == 149) cout << pho << "\ttheta, phi = " << *pho_theta << ", " << *pho_phi <<" -> ";
				r.Rotate_Photon(*pho_theta, *pho_phi);
				photon_event.Photons[pho].UnitVector = Get_UnitVector(*pho_theta, *pho_phi);
				// if (pho == 149) cout << *pho_theta << ", " << *pho_phi <<endl;
			}
		}



		double totalphotons = photon_event.Photons.size();
		if (print) cout << "\t\tTotal photons = " << totalphotons << endl;
		int counter = 0;
		for (int i = 0; i < photon_event.Photons.size(); i++)
		{
			auto& photon_i = photon_event.Photons[i];
			// if (counter == 149) cout << i << " theta, phi: " << photon_i.Theta << ", " << photon_i.Phi << " -> ";
			Simulate_PhotonPath(*d, photon_i, seed, false);
			if (photon_i.Flag == 1){
				ParEvent->Particles.at(photon_i.WhichParticle - old_ParEvent->Particles.size()).nPhotonsPassed -= 1;
			}
			// if (counter == 149) cout << photon_i.Theta << ", " << photon_i.Phi << endl;
			CheckForFlag(photon_event, i, "false");
			++counter;
		}

		embed_data(photon_event, *old_photon_event, *old_event_output, *ParEvent, *old_ParEvent, print);
		if (print){
			printf("\t\t(%f percent)\n\n", photon_event.Photons.size()/totalphotons*100.);
		}
		outputTree1->Fill();
		outputTree2->Fill();
	}

	fin1.cd();
	fin1.Close();
	fout1.cd();
	fout1.Write();
	fout1.Close();

	fin2->cd();
	fin2->Close();
	fout2->cd();
	fout2->Write();
	fout2->Close();

	return 0;

	// cout << "file: " << outputfile << endl;
}
