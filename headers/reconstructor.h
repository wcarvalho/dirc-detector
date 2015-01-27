//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>      // std::stringstream

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
#include "utility_library.h"
#include "FileProperties.h"
#include "functions.h"
#include "Simulate.h"
#include "fitter.h"

//------------------------------------------
//		Functions
//------------------------------------------

void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, bool print = false);
void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, std::vector<PhotonOut> photons, bool print = false);
void Reconstructed_Reflections(std::vector<PhotonOut> &pho, double theta, double phi, bool print = false);
// void XY_Reflections(std::vector<PhotonOut> &pho, Simulate sim, bool print = false);

std::string histName0(int event, int particle);
void removeFirstParticles(bool Remove, GeneratorOut*& event_output, int last);
void CreateHistogram_1D2D(int ev, int par, Analysis &A, std::vector<PhotonOut> &phos, int xbins, int ybins);

void CalculateParticleFits(double (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&), ParticleOut &P, Analysis &A, double range, double smear, bool print);