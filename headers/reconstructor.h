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
#include "TH2D.h"

//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, TH2D& h1, string Output);
void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, vector<PhotonOut> photons, TH2D& h1, string Output);
void FlipNegativePhotons(GeneratorOut*& event_output, string Output);
void FlipNegativePhoton(PhotonOut &photon, string Output);
void CreateHistogram(TH2D& h1, vector<PhotonOut> photons, string Output);


