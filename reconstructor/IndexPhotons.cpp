#include <vector>
#include "dirc_objects.h"
#include "Rotater.h"
#include "band_search_cases.h"

using namespace std;
double pi = 3.14159265358979312;

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
void best_photon_time_reconstruction(ParticleOut& P, double const& n, double const& x_distance, double const& theta, double const&phi, double const& recorded_photon_time, double& reconstructed_photon_time, double& time_difference){
	static double tp;
	static double beta;
	static double smallest_time_difference; smallest_time_difference = 1.e10; 							// difference between different reconstructed times dependent on associated type's mass (e.g. kaon vs. pion vs. electron...)
	static double time_in_dirc;
	auto masses = P.MassMap();


	time_in_dirc = fabs((1000*n/30)*(x_distance*1.e-2)/(sin(theta)*cos(phi)));

	// cout << endl;
	for (auto i = masses.begin(); i != masses.end(); ++i){
		double& mass = i->second;
		beta = P.CalculateBeta(mass);
		tp = (1000*P.path_to_dirc())/(beta*30);
		reconstructed_photon_time = tp + time_in_dirc;
		time_difference = recorded_photon_time - reconstructed_photon_time;
		if (time_difference < smallest_time_difference) smallest_time_difference = time_difference;
	}

}
bool isPhotonInTimeBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& x_distance_back, double const& x_distance_forward, Detector const& d, ParticleOut & P, double sigma, bool print){
	static Rotater r; r.Feed_Particle(P.Theta, P.Phi);

	static double time_difference;
	static double min_time_difference; min_time_difference = 10.e8;							// difference between recorded time and reconstructed time
	static double max_time_difference = 0;
	static double reconstructed_photon_time;
	// static int chosen_indx;
	static int count = 0;
	for (auto &i: photonset){
		cout << count << endl; ++count;
		auto& photon = photons.at(i);
		auto theta = photon.Theta;
		auto phi = photon.Phi;
		r.Rotate_Photon(theta, phi);
		// if (theta > pi/2) continue;
		static double x_distance;
		if (photon.GetWall() == Photon::FRONT)
			x_distance = x_distance_forward;
		else
			x_distance = x_distance_back;

		best_photon_time_reconstruction(P, d.n, x_distance, theta, phi, photon.Time_Traveled, reconstructed_photon_time, time_difference);

		cout << "recorded photon time =  " << photon.Time_Traveled << endl;
		cout <<	"reconstructed_photon_time = " << reconstructed_photon_time << endl;
		if (fabs(time_difference) < fabs(min_time_difference)){
			min_time_difference = time_difference;
		}
		if (fabs(time_difference) > fabs(max_time_difference)){
			max_time_difference = time_difference;
		}

	}
	cout << "\t- min_time_difference = " << min_time_difference << endl;
	cout << "\t- max_time_difference = " << max_time_difference << endl;
	cout << "\t\t- 5sigma = " << 5*sigma << endl;


	if (fabs(min_time_difference) < 5*sigma)
		return true;
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

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, TH2D& h, double const& smear, double const& time_smear, vector<int> const& cases, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, Detector const& d, bool const& print){

	void (*findThetaBand)(ParticleOut &, TH2D const&, double const&, double const&, double&, double&, vec_pair const&, bool const&);
	switch(band_search_case){
		case 1: findThetaBand=&TallestBinContent; break;
	}

	static double theta_center_min = 0.;
	static double theta_center_max = 0.;
	findThetaBand(particle, h, smear, band_search_width, theta_center_min, theta_center_max, expected_photons, print);

	static double x_distance_back;
	static double x_distance_forward;

	x_distance(particle, x_distance_back, x_distance_forward);

	int nphotons = photons.size()/4;
	vector<int> photonset{0, 0, 0, 0};
	for (unsigned i = 0; i < nphotons; ++i){

		getCurrentPhotonSet(i, nphotons, photonset);

		static bool passedBandTest;
		passedBandTest = true;
		for (auto&c : cases){
			switch(c){
				case 1:
					passedBandTest *= isPhotonInThetaBand(photons, photonset, theta_center_min, theta_center_max, print);
					break;
				case 2: passedBandTest *= isPhotonInTimeBand(photons, photonset, x_distance_back, x_distance_forward, d, particle, time_smear, print);
					break;
			}
		}

		if ( passedBandTest ){
			indexSet(index, photonset, photons_per_particle, particle_index, print);
		}
		// diagnostic_print(index, particle_index, photons, photonset, theta_center_min, theta_center_max, min_photon_time, max_photon_time);
	}

	return;
}