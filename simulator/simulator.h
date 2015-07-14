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
#include "../headers/functions.h"
#include "../headers/Parameters.h"
#include "Simulate.h"
#include "../headers/file-properties.h"

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

void Simulate_PhotonPath(Detector &d, Photon &photon, double smear = .01, bool print = false);
void Simulate_ParticlePath(Detector const& d, Particle &particle, int parnum, PhotonEvent &photon_event, int& seed, bool print);
void CheckForFlag(PhotonEvent &photon_event, int &iterator, string Output);
void CheckAngel(Detector d, Photon &photon, string Output);
void Check_PhotonDirection(double v, TVector3 &normal_vector);
void FillTree(TTree &tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output, GeneratorOut* event_outputCopy = 0, bool Append = false, bool quiet = false);
void Pull_Photons(PhotonEvent photon_event, GeneratorOut& event_output, string Output);
