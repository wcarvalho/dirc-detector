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
		// cout << i << "\t" << theta << ", " << phi << endl;
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

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, double const& band_search_width, double const& emission_angle, unordered_map <int, int>& photons_per_particle, bool const& print){


	static double theta_center_min = 0.;
	static double theta_center_max = 0.;
	theta_center_min = emission_angle - band_search_width;
	theta_center_max = emission_angle + band_search_width;

	static double min_photon_time = 0.;
	static double max_photon_time = 0.;
	findTimeBand(min_photon_time, max_photon_time, emission_angle, particle);

	int nphotons = photons.size()/4;
	vector<int> photonset{0, 0, 0, 0};
	for (unsigned i = 0; i < nphotons; ++i){

		getCurrentPhotonSet(i, nphotons, photonset);

		bool inThetaBand = isPhotonInThetaBand(photons, photonset, theta_center_min, theta_center_max, print);

		bool inTimeBand = isPhotonInTimeBand(photons, photonset, min_photon_time, max_photon_time, print);

		// inTimeBand = true;

		if ( inThetaBand && inTimeBand ){
			indexSet(index, photonset, photons_per_particle, particle_index, print);
		}
		// diagnostic_print(index, particle_index, photons, photonset, theta_center_min, theta_center_max, min_photon_time, max_photon_time);
	}

	return;
}