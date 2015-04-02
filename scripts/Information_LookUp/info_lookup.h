#include <iostream>
#include <vector>
#include <string>
#include <iomanip>

#include "TFile.h"
#include "TTree.h"
#include "TMath.h"

#include "Simulate.h"
#include "dirc_objects.h"
#include "event_parsers.h"

using namespace std;

void skip(stringstream& ss, const char& str){
	while (ss.peek() ==  str) ss.ignore();
}

// string nextword(stringstream& ss){
// 	skip(ss, ','); skip(ss, ' ');
// 	string unfinished_word;
// 	ss >> unfinished_word;
// 	int comma_pos = unfinished_word.find_first_of(",");
// 	int space_pos = unfinished_word.find_first_of(" ");

// 	cout << "__" << unfinished_word <<"___\n";
// 	cout << "comma_pos = " << comma_pos << endl;
// 	cout << "space_pos = " << space_pos << endl;
// 	if (comma_pos < space_pos)
// 		return unfinished_word.substr(0, comma_pos-1);
// 	else
// 		return unfinished_word.substr(0, space_pos-1);
// }

template <typename T>
void push_ss(T val, vector<string> & responses){
	stringstream ss;
	ss << val;
	responses.push_back(ss.str());
}

template <typename T>
bool fill_from_ss(stringstream& ss, T& val, vector<string>& responses){
	ss >> val;
	skip(ss, ','); skip(ss, ' ');
	push_ss(val, responses);
	if (ss.tellg() == -1 )
		return false;
	else
		return true;

}

void parse_answer(const string& answer, int& event,	int& particle, string& reconstruction, vector<string>& responses){

	responses.clear();
	stringstream ss(answer);
	if (!fill_from_ss(ss, event, responses)) return;
	if (!fill_from_ss(ss, particle, responses)) return;
	if (!fill_from_ss(ss, reconstruction, responses)) return;
}


void get_particle_recon(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction, Particle*& par, const TrackRecon*& recon){

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

}
void print_reconstruction_info(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

	Particle* par = 0;
	const TrackRecon *recon = 0;
	get_particle_recon(t1, t2, pars, recons, event, particle, reconstruction, par, recon);

  int reconpos = -1;
  for (unsigned i = 0; i < recon->Options.size(); ++i){
  	if (recon->Options.at(i) == reconstruction){
  		reconpos = i;
  		break;
  	}
  }
  if (reconpos == -1) return;

  cout << reconstruction << endl;
  cout << "N Photons: ";
  cout << "calculated = " << recon->ExpectedNumber.at(reconpos) << "\t";
  cout << "from fit = " << recon->Areas.at(reconpos) << "\t";
  cout << "nSigma = " << recon->delSigArea.at(reconpos) << "\n";

	auto expected_angle_map = par->EmissionAngleMap();

  cout << "Theta: ";
  cout << "calculated = " << expected_angle_map[reconstruction] << "\t";
  cout << "from fit = " << recon->Params.at(reconpos).at(1) << "\t";
  cout << "nSigma = " << recon->delSigTheta.at(reconpos) << "\n\n";

}

void print_particle_info(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

	Particle* par = 0;
	const TrackRecon *recon = 0;
	get_particle_recon(t1, t2, pars, recons, event, particle, reconstruction, par, recon);

	cout << "particle " << particle << ": ";

  cout << par->name << "\t";
  cout << "Eta = " << std::setprecision(3) << par->Eta << "\t";
  cout << "pt = " << std::setprecision(3) << par->pt << "\t";
  cout << "Momentum = " << std::setprecision(3) << par->CalculateMomentum() << "\n";

  cout << "X = " << std::setprecision(4) << par->X << "\t";
  cout << "Y = " << std::setprecision(4) << par->Y << "\n";

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
  cout << "\n";
	for (unsigned i = 0; i < recon->Options.size(); ++i){
		print_reconstruction_info(t1, t2, pars, recons, event, particle, recon->Options.at(i));
	}
	for (unsigned i = 0; i < 60; ++i)
		cout << "_";
	cout << endl;

}

void print_event_info(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

	t1.GetEntry(event);
	t2.GetEntry(event);

	for (unsigned i = 0; i < pars.size(); ++i)
		print_particle_info(t1, t2, pars, recons, event, i, reconstruction);
}

void print_upto_reconstruction(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

	t1.GetEntry(event);
	t2.GetEntry(event);
	Particle* par = 0;
	const TrackRecon *recon = 0;
	get_particle_recon(t1, t2, pars, recons, event, particle, reconstruction, par, recon);

	cout << "particle " << particle << ": ";

  cout << par->name << "\t";
  cout << "Eta = " << std::setprecision(3) << par->Eta << "\t";
  cout << "pt = " << std::setprecision(3) << par->pt << "\t";
  cout << "Momentum = " << std::setprecision(3) << par->CalculateMomentum() << "\n";

  cout << "X = " << std::setprecision(4) << par->X << "\t";
  cout << "Y = " << std::setprecision(4) << par->Y << "\n";

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
  cout << "\n";

	print_reconstruction_info(t1, t2, pars, recons, event, particle, reconstruction);
	for (unsigned i = 0; i < 60; ++i)
		cout << "_";
	cout << endl;

}

// void print_data(TTree& t1, TTree& t2, vector<Particle>& pars, vector<TrackRecon>& recons, const int& event, const int& particle, const string& reconstruction){

// 	t1.GetEntry(event);
// 	t2.GetEntry(event);

// 	Particle* par = 0;
// 	const TrackRecon *recon = 0;
// 	try{
// 		int nsize = pars.size();
// 		dirc::matchDataSize(t1, t2, recons, pars, false);
// 		int ndif = nsize - pars.size();
// 		if (ndif >0 ) cout << "warning, index shifted by " << ndif << endl;
// 		par = &pars.at(particle);
// 		recon = &recons.at(particle);
// 	}
//   catch (const std::out_of_range& oor) {
//   	cout << "This event has " << pars.size() << " particles\n";
// 	  std::cerr << "Out of Range error: " << oor.what() << '\n';
// 	  return;
//   }

// 	cout << "event = " << event << "\t";
// 	cout << "particle = " << particle << "\t";
// 	cout << "reconstruction = " << reconstruction << endl;

//   cout << "particle type = " << par->name << "\n";
//   cout << "Eta = " << par->Eta << "\n";
//   cout << "pt = " << par->pt << "\n";
//   cout << "Momentum = " << par->CalculateMomentum() << "\n\n";

//   cout << "X = " << par->X << "\t";
//   cout << "Y = " << par->Y << "\n";

//   cout << "Theta = " << par->Theta << "\t";
//   cout << "Phi = " << par->Phi << "\t";

// 	static Detector d;
// 	static Simulate SimPar(0,0);
// 	SimPar.SetAngle(par->Theta, par->Phi);
// 	SimPar.SetDim(d.Length, d.Width, d.Height);
// 	SimPar.SetStart(par->X, par->Y, 0);
// 	SimPar.DistancetoWalls();
// 	SimPar.WhichWall();

//   cout << "wall = ";
//   switch(SimPar.wall){
// 	  case 1: cout << "X\n"; break;
// 	  case 2: cout << "Y\n"; break;
// 	  case 3: cout << "Z\n"; break;
//   }
//   cout << endl;
//   int reconpos;
//   for (unsigned i = 0; i < recon->Options.size(); ++i)
//   	if (recon->Options.at(i) == reconstruction){
//   		reconpos = i;
//   		break;
//   	}

// 	// int reconpos = std::find(recon->Options.begin(), recon->Options.end(), reconstruction)!=recon->Options.end();

//   cout << "Expected Number of Photons: ";
//   cout << "calculated = " << std::setfill(' ') <<std::setw(10) << recon->ExpectedNumber.at(reconpos) << endl;
//   cout << "Expected Number of Photons: ";
//   cout << "from fit = " << std::setfill(' ') <<std::setw(10) << recon->Areas.at(reconpos) << endl;
//   cout << "Expected Number of Photons: nSigma = " << std::setfill(' ') <<std::setw(10) << recon->delSigArea.at(reconpos) << "\n";

// 	auto expected_angle_map = par->EmissionAngleMap();

//   cout << "Expected Theta: ";
//   cout << "calculated = " << std::setfill(' ') <<std::setw(10) << expected_angle_map[reconstruction] << endl;
//   cout << "Expected Theta: ";
//   cout << "from fit = " << std::setfill(' ') <<std::setw(10) << recon->Params.at(reconpos).at(1) << endl;
//   cout << "Expected Theta: nSigma = " << std::setfill(' ') <<std::setw(10) << recon->delSigTheta.at(reconpos) << endl;


// }