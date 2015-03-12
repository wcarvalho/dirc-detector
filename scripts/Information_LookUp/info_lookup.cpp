#include <vector>
#include <string>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include "Simulate.h"
#include "dirc_objects.h"
#include "event_parsers.h"

#include <tclap/CmdLine.h>
using namespace std;

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
	string question = "\nWhich event, particle, and reconstruction would you like to look up information for? (please use commas for seperation; '.q' to close)\n";
	bool asking = true;
	string answer;
	int event = 0, particle = 0;
	string reconstruction = "";
	while (asking){
		cout << question;
		getline(cin, answer);
		if (!(answer.find(".q") == string::npos)) break;
		parse_answer(answer, event, particle, reconstruction);
		print_data(*t1, *t2, *pars, *recons, event, particle, reconstruction);
	}
	return 0;
}

void skip(stringstream& ss, const char& str){
	while (ss.peek() ==  str) ss.ignore();
}

void parse_answer(const string& answer, int& event, int& particle, string& reconstruction){

	stringstream ss(answer);
	ss >> event;
	skip(ss, ','); skip(ss, ' ');
	ss >> particle;
	skip(ss, ','); skip(ss, ' ');
	ss >> reconstruction;
	skip(ss, ','); skip(ss, ' ');
}

void print_data(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

	t1.GetEntry(event);
	t2.GetEntry(event);

	Particle* par = 0;
	const TrackRecon *recon = 0;
	try{
		int nsize = pars.size();
		dirc::matchDataSize(t1, t2, recons, pars, false);
		int ndif = nsize - pars.size();
		if (ndif >0 ) cout << "warning, index shifted by " << ndif << endl;
		par = &pars.at(particle);
		recon = &recons.at(particle);
	}
  catch (const std::out_of_range& oor) {
  	cout << "This event has " << pars.size() << " particles\n";
	  std::cerr << "Out of Range error: " << oor.what() << '\n';
	  return;
  }

	cout << "event = " << event << "\t";
	cout << "particle = " << particle << "\t";
	cout << "reconstruction = " << reconstruction << endl;

  cout << "particle type = " << par->name << "\n";
  cout << "Eta = " << par->Eta << "\n";
  cout << "pt = " << par->pt << "\n";
  cout << "Momentum = " << par->CalculateMomentum() << "\n\n";

  cout << "X = " << par->X << "\t";
  cout << "Y = " << par->Y << "\n";

  cout << "Theta = " << par->Theta << "\t";
  cout << "Phi = " << par->Phi << "\t";

	static Detector d;
	static Simulate SimPar(0,0);
	SimPar.SetAngle(par->Theta, par->Phi);
	SimPar.SetDim(d.Length, d.Width, d.Height);
	SimPar.SetStart(par->X, par->Y, 0);
	SimPar.DistancetoWalls();
	SimPar.WhichWall();

  cout << "wall = ";
  switch(SimPar.wall){
	  case 1: cout << "X\n"; break;
	  case 2: cout << "Y\n"; break;
	  case 3: cout << "Z\n"; break;
  }
  cout << endl;
  int reconpos;
  for (unsigned i = 0; i < recon->Options.size(); ++i)
  	if (recon->Options.at(i) == reconstruction){
  		reconpos = i;
  		break;
  	}

	// int reconpos = std::find(recon->Options.begin(), recon->Options.end(), reconstruction)!=recon->Options.end();

  cout << "Expected Number of Photons: ";
  cout << "calculated = " << std::setfill(' ') <<std::setw(10) << recon->ExpectedNumber.at(reconpos) << endl;
  cout << "Expected Number of Photons: ";
  cout << "from fit = " << std::setfill(' ') <<std::setw(10) << recon->Areas.at(reconpos) << endl;
  cout << "Expected Number of Photons: nSigma = " << std::setfill(' ') <<std::setw(10) << recon->delSigArea.at(reconpos) << "\n";

	auto expected_angle_map = par->EmissionAngleMap();

  cout << "Expected Theta: ";
  cout << "calculated = " << std::setfill(' ') <<std::setw(10) << expected_angle_map[reconstruction] << endl;
  cout << "Expected Theta: ";
  cout << "from fit = " << std::setfill(' ') <<std::setw(10) << recon->Params.at(reconpos).at(1) << endl;
  cout << "Expected Theta: nSigma = " << std::setfill(' ') <<std::setw(10) << recon->delSigTheta.at(reconpos) << endl;


}