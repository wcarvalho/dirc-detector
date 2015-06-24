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


typedef std::vector<PhotonOut> Photons;
typedef std::map< std::string, pair<double, double> > vec_pair;
typedef std::vector< Photons > Photon_Sets;

void getExpectedPhotonMap(vector<ParticleOut> & pars, unordered_map <int, vec_pair>& expectedPhotonMap, std::pair<double, double> (*ExpectedNumberofPhotons)(double const&, double const&, double const&, double const&, double const&));


Photons reconstruct_photons(Photons const& set);
Photons rotate_photons_into_particle_frame(double const theta, double const phi, Photons const& original_photons);

void check_reconstructed_photons(Photons& photons);

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, TH2D& h, double const& smear, vector<int> const& cases, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, bool const& print);

TH2D histogram_photon_angles(int const& event, int const& particle, vector<PhotonOut> const& photons);

TH1D* ReducedHistogram(vector<PhotonOut> const& photons, TH2D const& h2, vector<int> const& index, int const& particle_index);




void reset_photons_per_particle(unordered_map <int, int>& photons_per_particle, unsigned const npar);
void reset_Tracks(TrackRecons &Tracks, unsigned const npars, unsigned const nphotons);
void reset_photons_in_different_frames(Photon_Sets& photons_in_different_frames, unsigned const npars);


void ReconstructEvent(Reconstruction &reconstruction, GeneratorOut*& event_output, bool print = false);
void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, std::vector<PhotonOut> photons, bool print = false);
void Reconstructed_Reflections(std::vector<PhotonOut> &pho, double theta, double phi, bool print = false);
// void XY_Reflections(std::vector<PhotonOut> &pho, Simulate sim, bool print = false);

std::string histName0(int event, int particle);
void removeFirstParticles(GeneratorOut*& event_output, int last, bool print);
void CreateHistogram_1D2D(int ev, int par, Analysis &A, std::vector<PhotonOut> &phos, int xbins, int ybins);
void IndexPhotons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, Analysis & A, double const& smear, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photon_overlap, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, map<string,double> const& anglemap, bool const& print);

TH1D* CreateReducedHistogram(vector<PhotonOut> const& photons, vector<int> const& index, int const particle_index, string histname, int nbins, double xlow, double xhi);

void CalculateParticleFits(TH1D &histogram, ParticleOut &P, TrackRecon &T, vec_pair& expected_photons, const int particle_index, double smear, bool print);



// void CalculateParticleFits(TH1D &histogram, ParticleOut &P, const vector<PhotonOut>& phos, Analysis &A, const int particle_index, double smear, int const& loss, vec_pair& expected_photons, bool print);
TH1D rebinHistogram(TH1D& h, double minimum = 10.);


bool passed_index_photons_condition(ParticleOut & P, double momentum_indexing_threshold);

void FitGaussianPlusConstant(TH1D &h, double xlow, double xhi, double centerbounds_low, double centerbounds_hi, double widthbounds_low, double widthbounds_hi, double& center, double& width, double& constant, double& height, unsigned converge_at = 4);