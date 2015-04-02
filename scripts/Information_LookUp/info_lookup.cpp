#include "info_lookup.h"
#include <tclap/CmdLine.h>

void parse_answer(const string& answer, int& event, int& particle, string& reconstruction);
void print_data(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction);

int main(int argc, char const *argv[])
{

	string cheatdata;
	string reconstructiondata;

TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> reconstructileFileArg("r","reconstruction","file with reconstruction data",false,"reconstruction.root","string", cmd);

	TCLAP::ValueArg<std::string> particleFileArg("p","particle-file","file with particle data (cheat data)",false,"cheat.root","string", cmd);

	cmd.parse( argc, argv );
	reconstructiondata = reconstructileFileArg.getValue();
	cheatdata = particleFileArg.getValue();

}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	double pi = TMath::Pi();

	ParticleEvent *originals = 0;
	TrackRecons *reconstructions = 0;

	string file1 = cheatdata;
	string file2 = reconstructiondata;

	TCanvas C("C", "C", 1000, 600);

	TFile f1(file1.c_str(), "read");
	TTree *t1 = (TTree*)f1.Get("cheat_info");
	t1 -> SetBranchAddress("Particle Event", &originals);

	TFile f2(file2.c_str(), "read");
	TTree *t2 = (TTree*)f2.Get("identifications");
	t2 -> SetBranchAddress("guesses", &reconstructions);

	vector<Particle> *pars = &originals->Particles;
	vector<TrackRecon> *recons = &reconstructions->Recon;


	cout << "This event has " << t1->GetEntries() << " events\n";
	string question = "\nWhich event, particle, and/or reconstruction would you like to look up information for? (please use commas for seperation; '.q' to close)\n";
	bool asking = true;
	string answer;
	int event = 0, particle = 0;
	string reconstruction = "";
	vector<string> responses;
	while (asking){
		cout << question;
		getline(cin, answer);
		if (!(answer.find(".q") == string::npos)) break;
		parse_answer(answer, event, particle, reconstruction, responses);

		if (responses.size() == 1) print_event_info(*t1, *t2, *pars, *recons, event, particle, reconstruction);

		if (responses.size() == 2){
			t1->GetEntry(event);
			t2->GetEntry(event);
			print_particle_info(*t1, *t2, *pars, *recons, event, particle, reconstruction);
		}

		if (responses.size() == 3){
			print_upto_reconstruction(*t1, *t2, *pars, *recons, event, particle, reconstruction);
		}
	}
	return 0;
}

