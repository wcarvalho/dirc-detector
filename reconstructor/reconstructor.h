//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>      // std::stringstream
#include <unordered_map>
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
void removeFirstParticles(GeneratorOut*& event_output, int last, bool print);
void CreateHistogram_1D2D(int ev, int par, Analysis &A, std::vector<PhotonOut> &phos, int xbins, int ybins);
void IndexPhotons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, Analysis & A, double const& smear, unsigned const& band_search_case, unordered_map <int, int>& photon_overlap, bool const& print);

TH1D* ReducedHistogram(vector<PhotonOut> const& photons, Analysis const& A, int const& particle_index);
TH1D* CreateReducedHistogram(vector<PhotonOut> const& photons, vector<int> const& index, int const particle_index, string histname, int nbins, double xlow, double xhi);
void CalculateParticleFits(std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&), TH1D &histogram, ParticleOut &P, const vector<PhotonOut>& phos, Analysis &A, const int particle_index, double smear, int const& loss, bool print);

bool passed_index_photons_condition(ParticleOut & P, double momentum_indexing_threshold);

void FitGaussianPlusConstant(TH1D &h, double xlow, double xhi, double centerbounds_low, double centerbounds_hi, double widthbounds_low, double widthbounds_hi, double& center, double& width, double& constant, double& height, unsigned converge_at = 4);