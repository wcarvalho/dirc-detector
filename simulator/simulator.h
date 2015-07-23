#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"

//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
#include "Parameters.h"
#include "Simulate.h"
#include "FileProperties.h"

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

typedef vector<Particle> Particles;
typedef vector<Photon> Photons;

void SimulateParticles(Detector const& d, Particles& particles, PhotonEvent& photon_event, int& seed, bool print);
void Simulate_ParticlePath(Detector const& d, Particle &particle, int parnum, PhotonEvent &photon_event, int& seed, bool print);

void SimulatePhotons(Detector& d, PhotonEvent& photon_event, Particles particles, int& seed, double const& quantum_efficiency, bool print);
void Simulate_PhotonPath(Detector &d, Photon &photon, double smear = .01, bool print = false);


void CheckForFlag(PhotonEvent &photon_event, int &iterator, string Output);
void CheckAngel(Detector d, Photon &photon, string Output);
void Check_PhotonDirection(double v, TVector3 &normal_vector);
void FillTree(TTree &tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output, GeneratorOut* event_outputCopy = 0, bool Append = false, bool quiet = false);
void Pull_Photons(PhotonEvent photon_event, GeneratorOut& event_output, string Output);
bool quantum_efficiency_cut(double const& quantum_efficiency, int& seed, bool print);
