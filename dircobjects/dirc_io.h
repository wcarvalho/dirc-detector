#include <string>
#include "TFile.h"
#include "TTree.h"
#include "dirc_objects.h"

void checkValid(const TFile& f);

void readInGeneratorData(TFile*& f, std::string filename, TTree*& t, GeneratorOut*& gen_out, Detector*& d){

	f = new TFile(filename.c_str(), "read");
	checkValid(*f);
	t = (TTree*)f->Get("sim_out");
	t->SetBranchAddress("simEvent", &gen_out);
	t->SetBranchAddress("detector", &d);
}
void readInCheatData(TFile*& f, std::string filename, TTree*& t, ParticleEvent*& particle_data, PhotonEvent*& photon_data, Detector*& d){

	f = new TFile(filename.c_str(), "read"); checkValid(*f);
	t = (TTree*)f->Get("cheat_info");
	t->SetBranchAddress("Particle Event", &particle_data);
	t->SetBranchAddress("Photon Event", &photon_data);
	t->SetBranchAddress("detector", &d);
}
void readInCheatData(TFile*& f, std::string filename, TTree*& t, ParticleEvent*& particle_data, PhotonEvent*& photon_data){
	Detector *d = 0;
	readInCheatData(f, filename, t, particle_data, photon_data, d);
}
void readInReconstructionData(TFile*& f, std::string filename, TTree*& t, TrackRecons*& reconstructions){

	f = new TFile(filename.c_str(), "read"); checkValid(*f);
	t = (TTree*)f->Get("identifications");
	t->SetBranchAddress("guesses", &reconstructions);
}
void checkValid(const TFile& f){
	if (!(f.IsOpen())) {
		std::cout << f.GetName() << " invalid!\n";
		exit(1);
	}
}