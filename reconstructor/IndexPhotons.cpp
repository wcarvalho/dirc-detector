#include <vector>
#include "dirc_objects.h"
#include "band_search_cases.h"
using namespace std;

void getCurrentPhotonSet(unsigned const& i, int const& nphotons, vector<int>& photonset){
	photonset.at(0) = i;
	photonset.at(1) = 3*i + nphotons;
	photonset.at(2) = 3*i + nphotons + 1;
	photonset.at(3) = 3*i + nphotons + 2;
}



bool setInsideBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& center_min, double const& center_max, bool print){
	static bool inside_band;
	inside_band = false;
	for (auto& i: photonset){
		auto& photon = photons.at(i);
		auto& theta = photon.Theta;
		auto& phi = photon.Phi;
		// cout << i << "\t" << theta << ", " << phi << endl;
		if ((theta<center_max)&&(theta>center_min)){
			inside_band = true; break;
		}
	}		
	return inside_band;
}

void indexSet(vector<int>& index, vector<int> const& photonset, unordered_map <int, int>& photon_overlap, int const& particle_index, bool print){

	for (auto& i: photonset){
		auto& current_index = index.at(i);
		// cout << i << "\tcurrent_index: " << current_index << " -> ";
		if (current_index > -1	) {
			++photon_overlap[current_index];
			++photon_overlap[particle_index];
			current_index = -1;
		}
		else if (current_index == -1) ++photon_overlap[particle_index];
		else current_index = particle_index;
		// cout << current_index << endl;
	}
}

void diagnostic_print(vector<int> const& index, int const& particle_index, vector<PhotonOut> const& photons, vector<int> const& photonset, double const& center_min, double const& center_max){

	for (auto& i: photonset){
		auto& photon = photons.at(i);
		auto& theta = photon.Theta;
		auto& phi = photon.Phi;
		auto& current_index = index.at(i);
		// if (current_index == -1){
			cout << "frame: " << particle_index << "\t"<< i << "\tindex = " << current_index << " angle = " << theta << ", " <<  phi << endl;
		// }
	}
}

void IndexPhotons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, Analysis & A, double const& smear, unsigned const& band_search_case, unordered_map <int, int>& photon_overlap, bool const& print){

	void (*GetPhotonBand)(ParticleOut &, TH2D const&, double const&, double&, double&, bool const&);
	switch(band_search_case){
		case 1: GetPhotonBand=&TallestBinContent; break;
		// case 2: GetPhotonBand=&HoughTransform; // FIXME::error linking libraries
	}
	const TH2D& h = A.Hists2D.back();
	static double center_min = 0.;
	static double center_max = 0.;
	GetPhotonBand(particle, h, smear, center_min, center_max, print);

	int nphotons = photons.size()/4;
	vector<int> photonset{0, 0, 0, 0};
	for (unsigned i = 0; i < nphotons; ++i){
		static unsigned set_start;
		static unsigned set_end;
		getCurrentPhotonSet(i, nphotons, photonset);

		if ( setInsideBand(photons, photonset, center_min, center_max, print) ){
			indexSet(A.index, photonset, photon_overlap, particle_index, print);
		}
		// cout << "center_min = " << center_min << endl;
		// cout << "center_max = " << center_max << endl;
		// diagnostic_print(A.index, particle_index, photons, photonset, center_min, center_max);

		// i=set_end;
	}

	return;
}