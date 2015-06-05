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
bool inBand(double const& min, double const& max, double const& val){
	if ((val < max) && (val > min)) return true;
	else return false;
}
bool isPhotonInThetaBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& theta_center_min, double const& theta_center_max, bool print){
	static bool inside_band;
	inside_band = false;
	for (auto& i: photonset){
		auto& photon = photons.at(i);
		auto& theta = photon.Theta;
		auto& phi = photon.Phi;
		if ((theta<theta_center_max)&&(theta>theta_center_min)){
			inside_band = true; break;
		}
	}
	return inside_band;
}
bool isPhotonInTimeBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& time_min, double const& time_max, bool print){

	static bool inside_band;
	inside_band = false;
	static unsigned indx;
	indx = photonset.at(0);

	auto& photon = photons.at(indx);
	if ( inBand(time_min, time_max, photon.Time_Traveled) ){
		return true;
	}
	else
		return false;

}
void indexSet(vector<int>& index, vector<int> const& photonset, unordered_map <int, int>& photons_per_particle, int const& particle_index, bool print){

	for (auto& i: photonset){
		auto& current_index = index.at(i);

		if (current_index > -1	) {
			--photons_per_particle[current_index];
			current_index = -1;
		}
		else if (current_index < -1	){
			current_index = particle_index;
			++photons_per_particle[particle_index];
		}
	}

}
void diagnostic_print(vector<int> const& index, int const& particle_index, vector<PhotonOut> const& photons, vector<int> const& photonset, double const& theta_center_min, double const& theta_center_max, double const& time_min, double const& time_max){

	for (auto& i: photonset){
		auto& photon = photons.at(i);
		auto& theta = photon.Theta;
		auto& phi = photon.Phi;
		auto& current_index = index.at(i);
		auto& t = photon.Time_Traveled;
		// if (current_index == -1){
			cout << "frame: " << particle_index << "\tindex "<< i << "\tindex = " << current_index << endl;
			cout << "\t\t\ttheta = " << theta << " in band: " <<  theta_center_min << ", " << theta_center_max << endl;
			cout << "\t\t\ttime = " << t << " in band: " <<  time_min << ", " << time_max << endl;
		// }
	}
}







typedef std::unordered_map<int, bool(*)(vector<PhotonOut> const&, vector<int> const&, double const&, double const&, bool)> band_case_map;

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, TH2D& h, double const& smear, vector<int> const& cases, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, bool const& print){

	void (*findThetaBand)(ParticleOut &, TH2D const&, double const&, double const&, double&, double&, vec_pair const&, bool const&);
	switch(band_search_case){
		case 1: findThetaBand=&TallestBinContent; break;
	}

	static double theta_center_min = 0.;
	static double theta_center_max = 0.;
	findThetaBand(particle, h, smear, band_search_width, theta_center_min, theta_center_max, expected_photons, print);

	static double min_photon_time = 0.;
	static double max_photon_time = 0.;
	findTimeBand(min_photon_time, max_photon_time, particle);

	static band_case_map case_map;
	case_map[1] = &isPhotonInThetaBand;
	case_map[2] = &isPhotonInTimeBand;

	int nphotons = photons.size()/4;
	vector<int> photonset{0, 0, 0, 0};
	for (unsigned i = 0; i < nphotons; ++i){

		getCurrentPhotonSet(i, nphotons, photonset);

		bool passedBandTest = true;
		for (auto&c : cases){
			static double max, min;
			switch(c){
				case 1: min = theta_center_min; max = theta_center_max; break;
				case 2: min = min_photon_time; max = max_photon_time; break;
			}
			passedBandTest *= case_map[c](photons, photonset, min, max, print);
		}

		// bool inThetaBand = isPhotonInThetaBand(photons, photonset, theta_center_min, theta_center_max, print);

		// bool inTimeBand = isPhotonInTimeBand(photons, photonset, min_photon_time, max_photon_time, print);

		// inTimeBand  = true;
		if ( passedBandTest ){
			indexSet(index, photonset, photons_per_particle, particle_index, print);
		}
		// diagnostic_print(index, particle_index, photons, photonset, theta_center_min, theta_center_max, min_photon_time, max_photon_time);
	}

	return;
}