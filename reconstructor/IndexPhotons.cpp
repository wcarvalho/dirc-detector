#include <vector>
#include "dirc_objects.h"
#include "band_search_cases.h"
using namespace std;

void IndexPhotons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, Analysis & A, double const& smear, unsigned const& band_search_case, unordered_map <int, int> photon_overlap, bool const& print){

	void (*GetPhotonBand)(ParticleOut &, TH2D const&, double const&, double&, double&, bool const&);
	switch(band_search_case){
		case 1: GetPhotonBand=&TallestBinContent; break;
		// case 2: GetPhotonBand=&HoughTransform; // FIXME::error linking libraries
		// case 2: GetPhotonBand=&HoughTransform; // FIXME::error linking libraries
	}
	const TH2D& h = A.Hists2D.back();
	double center_min = 1.e4;
	double center_max = 0;
	GetPhotonBand(particle, h, smear, center_min, center_max, print);

	// cout << "center_min = " << center_min << endl;
	// cout << "center_max = " << center_max << endl;
	for (unsigned i = 0; i < photons.size(); ++i){
		auto const& photon = photons.at(i);
		double const& theta = photon.Theta;
		if ((theta>center_max)||(theta<center_min)) continue;
		auto& current_index = A.index.at(i);
		if (current_index >= 0) current_index = -1;
		else current_index = particle_index;
	}
}