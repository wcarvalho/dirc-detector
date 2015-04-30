#include "reconstructor.h"

void reset_photons_per_particle(unordered_map <int, int>& photons_per_particle, unsigned const npar){
  photons_per_particle.clear();
  for (unsigned i = 0; i < npar; ++i)
  	photons_per_particle[i] = 0;
}

void reset_photons_in_different_frames(Photon_Sets& photons_in_different_frames, unsigned const npars){
	photons_in_different_frames.clear();
	photons_in_different_frames.resize(npars);
}

void reset_Tracks(TrackRecons &Tracks, unsigned const npars, unsigned const nphotons){

  Tracks.Recon.clear();
  Tracks.Recon.resize(npars);
	vector<int>& index = Tracks.index; 				// used to color photons
	index.clear();
	index.resize(nphotons, -10);

}