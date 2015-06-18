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
bool isPhotonInTimeBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& x_distance_back, double const& x_distance_forward, double const& particle_time_to_dirc, Detector const& d, ParticleOut const& P, bool print){

	static double sigma = 5;
	static Rotater r; r.Feed_Particle(P.Theta, P.Phi);

	static double time_in_dirc;
	static double total_time;
	static double time_difference;
	static double min_time_difference; min_time_difference = 10.e8;
	static string name = "reconstructed time - measured time";
	static TH1D h(name.c_str(), name.c_str(), 1000, -.2, .4);
	static int chosen_indx;
	static int count = 0;
	for (auto &i: photonset){
		++count;
		auto& photon = photons.at(i);
		auto theta = photon.Theta;
		auto phi = photon.Phi;
		r.Rotate_Photon(theta, phi);
		if (theta > pi/2) continue;
		static double x_distance;
		if (photon.GetWall() == Photon::FRONT)
			x_distance = x_distance_forward;
		else
			x_distance = x_distance_back;
		time_in_dirc = fabs((d.n/30)*(x_distance*1.e-2)/(sin(theta)*cos(phi)));
		// cout << "time_in_dirc = " << time_in_dirc << endl;
		total_time = time_in_dirc + particle_time_to_dirc;
		// cout << "total_time = " << total_time << endl;
		// cout << "photon.Time_Traveled = " << photon.Time_Traveled << endl;
		time_difference = total_time - photon.Time_Traveled;
		if (fabs(time_difference) < fabs(min_time_difference)){
			chosen_indx = i;
			min_time_difference = time_difference;
			if (fabs(min_time_difference) > .2){
				cout << "\nwall = " << photon.GetWall() << endl;
				cout << "x_distance = " << x_distance << endl;
				cout << "theta = " << theta << endl;
				cout << "phi = " << phi << endl;
				cout << "reconstructed time_in_dirc = " << time_in_dirc << endl;
				cout << "recorded      time_in_dirc = " << photon.Time_Traveled << endl;
			}
		}

	}



	if (fabs(min_time_difference) < sigma){
		h.Fill(min_time_difference);
		// cout << "count = " << count << ", size = " << photons.size() << endl;;
		if (count == (photons.size()) ){
			h.SaveAs("time_difference.root");
			count = 0;
		}
		return true;
	}
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

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, TH2D& h, double const& smear, vector<int> const& cases, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, Detector const& d, bool const& print){

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
	double particle_time_to_dirc = time_of_flight(particle);

	int nphotons = photons.size()/4;
	vector<int> photonset{0, 0, 0, 0};
	for (unsigned i = 0; i < nphotons; ++i){

		getCurrentPhotonSet(i, nphotons, photonset);

		static bool passedBandTest;
		passedBandTest = true;
		for (auto&c : cases){
			switch(c){
				case 1: passedBandTest *= isPhotonInThetaBand(photons, photonset, theta_center_min, theta_center_max, print);
				case 2: passedBandTest *= isPhotonInTimeBand(photons, photonset, x_distance_back, x_distance_forward, particle_time_to_dirc, d, particle, print);
			}
		}

		if ( passedBandTest ){
			indexSet(index, photonset, photons_per_particle, particle_index, print);
		}
		// diagnostic_print(index, particle_index, photons, photonset, theta_center_min, theta_center_max, min_photon_time, max_photon_time);
	}

	return;
}