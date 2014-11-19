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
#include "Simulate.h"
#include "file-properties.h"

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, Displayer disp);
void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, vector<PhotonOut> photons, Displayer &disp);
void Reconstructed_Reflections(vector<PhotonOut> &pho, double theta, double phi, Displayer disp);
void XY_Reflections(vector<PhotonOut> &pho, Simulate sim, Displayer disp);


