#include <string>

#include "TFile.h"

#include "dirc_io.h"
#include <tclap/CmdLine.h>

using namespace std;

bool equal(double v1, double v2){
	return (fabs(v1 - v2) < 1.e-9);
}

int main(int argc, char** argv){

string reconstructiondata;
string particledata;
string search_type;
string output;

double threshold;
double momentum;
vector<double> bounds {.5, .95};

bool print;


TCLAP::CmdLine cmd("Command description message", ' ', "0.1");
try{

	TCLAP::ValueArg<std::string> reconstructionFileArg("r","reconstruction","file with reconstruction data",true,"reconstruction.root","string", cmd);
	TCLAP::ValueArg<std::string> particleFileArg("p","particle","file with particle data",true,"particle.root","string", cmd);

	TCLAP::ValueArg<std::string> search_typeArg("t","type","type for which distribution will be generated",true,"theta","string", cmd);

	TCLAP::ValueArg<std::string> outputArg("o","output","output-file",false,"theta_distribution.root","string", cmd);

	TCLAP::MultiArg<double> boundsArg("b","bounds","plot bounds",false,"string", cmd);

	TCLAP::ValueArg<double> thresholdArg("T","threshold","",false,100,"double", cmd);

	TCLAP::ValueArg<double> momentumArg("m","momentum","",true,100,"double", cmd);


	TCLAP::SwitchArg verboseArg("v","verbose","", cmd, false);

	cmd.parse( argc, argv );

	reconstructiondata = reconstructionFileArg.getValue();
	particledata = particleFileArg.getValue();
	search_type = search_typeArg.getValue();
	momentum = momentumArg.getValue();
	output = outputArg.getValue();
	threshold = thresholdArg.getValue();
	print = verboseArg.getValue();

	if (boundsArg.isSet()) bounds = boundsArg.getValue();
}
catch( TCLAP::ArgException& e )
{ cout << "ERROR: " << e.error() << " " << e.argId() << endl; }

	TFile* f = 0;
	TTree* t = 0;
	TrackRecons* Tracks = 0;
	readInReconstructionData(f, reconstructiondata, t, Tracks);

	TFile* f2 = 0;
	TTree* t2 = 0;
	ParticleEvent* particle_event = 0;
	PhotonEvent* photon_event = 0;
	readInCheatData(f2, particledata, t2, particle_event, photon_event);
	double pi=TMath::Pi();

	TH1D h("h", "h", 10000, bounds.at(0), bounds.at(1));

	if (print) cout << "searching for " << search_type << endl;

	auto& particles = particle_event->Particles;

	for (unsigned ev = 0; ev < t->GetEntries(); ++ev){
		t->GetEntry(ev);
		t2->GetEntry(ev);
		if (print) cout << "Event " << ev << " with " << Tracks->Recon.size() << " particles\n";
		for (unsigned i = 0; i < particles.size(); ++i){
			auto& track = Tracks->Recon.at(i);
			auto& particle = particles.at(i);
			if (particle.name != search_type ) continue;
			if (!equal(particle.momentum, momentum)) continue;
			// check that the best reconstruction matches the particle type we're targetting
			static string best_fit; best_fit = track.getBestFit(threshold, print);
			if (best_fit == "" ) continue;
			static double theta;
			theta = track.getIntegralCenterAt(track.getIndexOf(best_fit));
			h.Fill(theta);
		}
	}

	TFile f1(output.c_str(), "recreate");
	f1.cd();
	h.Write();
	f1.Close();
	f->cd();
	f->Close();


}