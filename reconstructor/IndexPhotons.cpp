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
bool inBand(double const& min, double const& max, double const& val, bool print=false){
	if (print) cout << min << " : " << val << " : " << max << " ?\n";
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
		if (print) cout << "theta\n";
		if ( inBand(theta_center_min, theta_center_max, theta) ){
			inside_band = true; break;
		}
	}
	return inside_band;
}
void determine_reconstructed_photon_time(double const& td, double const& te, ParticleOut & P, double const& recorded_photon_time, double& best_tp, double& best_reconstructed_photon_time, double& time_difference){

	static double tp;
	static double beta;
	static double reconstructed_photon_time;
	static double smallest_time_difference;
	smallest_time_difference = 1.e10; 							// difference between different reconstructed times dependent on associated type's mass (e.g. kaon vs. pion vs. electron...)

	auto masses = P.MassMap();

	for (auto i = masses.begin(); i != masses.end(); ++i){
		double& mass = i->second;
		beta = P.CalculateBeta(mass);
		tp = (100*P.path_to_dirc())/(beta*3);
		reconstructed_photon_time = tp + td + te;
		time_difference = fabs(recorded_photon_time - reconstructed_photon_time);
		// cout << i->first << " tp = " << tp << endl;
		if (time_difference < smallest_time_difference){
			smallest_time_difference = time_difference;
			best_reconstructed_photon_time = reconstructed_photon_time;
			best_tp = tp;
			// cout << "\tchosen: " << i->first << endl;
			// cout << i->first << " recorded_photon_time = " << tp << " + " << time_in_dirc << endl;
		}
	}
	// exit(1);
}

void time_in_dirc(double const& x_distance, double const speed, double const& theta, double const& phi, double& time_dirc){
	static double photon_path_length;
	photon_path_length = 100*fabs(x_distance/(sin(theta)*cos(phi)));

	time_dirc = photon_path_length/speed;
}
void time_until_photon_emission(ParticleOut & P, double const x_emit, double& time_emission){

	double const& x_start = P.X;
	static double distance;
	static double particle_speed;

	particle_speed = 3*P.CalculateBeta(P.MassMap().begin()->second);
	distance = fabs(x_emit - x_start);
	// cout << "particle_speed = " << particle_speed << endl;
	// cout << "x_start = " << x_start << endl;
	// cout << "x_emit = " << x_emit << endl;
	time_in_dirc(distance, particle_speed, P.Theta, P.Phi, time_emission);

}

bool isPhotonInTimeBand(vector<PhotonOut> const& photons, vector<int> const& photonset, double const& x_distance_back, double const& x_distance_forward, Detector const& d, ParticleOut & P, double const& smear, double const& error, double& t_min, double& t_max, bool print){
	static Rotater r; r.Feed_Particle(P.Theta, P.Phi);

	static double time_difference;
	static double min_time_difference; min_time_difference = 10.e8;							// difference between recorded time and reconstructed time
	static double max_time_difference = 0;
	static double reconstructed_photon_time;
	// static int chosen_indx;

	static double td;
	static double te;
	static double tp;

	auto const& photon = photons.at(photonset.at(0));
	auto theta = photon.Theta;
	auto phi = photon.Phi;
	r.Rotate_Photon(theta, phi);
	// if (theta > pi/2) continue;
	static double x_distance;
	if (photon.GetWall() == Photon::FRONT)
		x_distance = x_distance_forward;
	else
		x_distance = x_distance_back;

	time_in_dirc(x_distance, 3/d.n, theta, phi, td);
	time_until_photon_emission(P, x_distance_back, te);

	static int count = 0;
	determine_reconstructed_photon_time(td, te, P, photon.Time_Traveled, tp,reconstructed_photon_time, time_difference);
	// cout << "photon "<< count << " td: " << td << endl; ++count;
	// cout << "\tte = " << te << endl;
	// cout << "\ttp = " << tp << endl;
	// cout << "\t\treconstructed: " << reconstructed_photon_time << endl;
	// h.Fill(time_difference);
	// h.Draw("COLZ");
	// C.SaveAs("time_dif.root");
	// if (time_difference > 200){
		// cout << count << " : " << theta << ", " << phi << endl; ++count;
		// cout << "\tx_distance = " << x_distance << endl;
		// cout << "\ttime_difference = " << time_difference << endl;
		// cout << "\trecorded photon time =  " << photon.Time_Traveled << endl;
		// cout <<	"\treconstructed_photon_time = " << reconstructed_photon_time << endl;
	// }
	if (fabs(time_difference) < fabs(min_time_difference)){
		min_time_difference = time_difference;
	}
	if (fabs(time_difference) > fabs(max_time_difference)){
		max_time_difference = time_difference;
	}

	// cout << "\t- min_time_difference = " << min_time_difference << endl;
	// cout << "\tmax_time_difference = " << max_time_difference << endl;
	// cout << "\t\t- 5smear = " << 5*smear << endl;
	t_min = reconstructed_photon_time - smear - error;
	t_max = reconstructed_photon_time + smear + error;

	// if (print) cout << "time\n";
	if ( inBand(t_min, t_max, photon.Time_Traveled) ) return true;
	else return false;
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

	static double time_center;
	static double time_sigma;
	static double theta_center;
	static double theta_sigma;

	time_center = (time_max + time_min)/2.;
	time_sigma = time_max - time_center;

	theta_center = (theta_center_max + theta_center_min)/2.;
	theta_sigma = theta_center_max - theta_center;

	cout << std::setfill ('-') << std::setw (19) << "-" << endl;
	for (auto& i: photonset){
		auto& photon = photons.at(i);
		auto& theta = photon.Theta;
		auto& phi = photon.Phi;
		auto& current_index = index.at(i);
		auto& t = photon.Time_Traveled;
		// if (current_index == -1){
			cout << "frame: " << particle_index << "\tphoton "<< i << "\tcolor = " << current_index << endl;


			cout << "\t\t\ttheta = " << theta << ", band: " <<  theta_center << "  \u00B1 " << theta_sigma;
			if (inBand(theta_center_min, theta_center_max, theta)) cout << " \u2605 \n";
			else cout << endl;

			cout << "\t\t\ttime = " << t << ", band: " <<  time_center << "  \u00B1 " << time_sigma;
			if (inBand(time_min, time_max, t)) cout << " \u2605 \n";
			else cout << endl;
		// }
	}
}

typedef std::unordered_map<int, bool(*)(vector<PhotonOut> const&, vector<int> const&, double const&, double const&, bool)> band_case_map;

void index_photons(ParticleOut & particle, int const& particle_index, vector<PhotonOut> const& photons, vector<int>& index, TH2D& h, double const& smear, double const& time_smear, double const& time_error, vector<int> const& cases, unsigned const& band_search_case, double const& band_search_width, unordered_map <int, int>& photons_per_particle, vec_pair const&expected_photons, Detector const& d, bool const& print){

	void (*findThetaBand)(ParticleOut &, TH2D const&, double const&, double const&, double&, double&, vec_pair const&, bool const&);
	switch(band_search_case){
		case 1: findThetaBand=&TallestBinContent; break;
	}

	static double theta_center_min = 0.;
	static double theta_center_max = 0.;
	static double min_photon_time = 0.;
	static double max_photon_time = 0.;
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
				case 2: passedBandTest *= isPhotonInTimeBand(photons, photonset, x_distance_back, x_distance_forward, d, particle, time_smear, time_error, min_photon_time, max_photon_time, print);
					break;
			}
			// if (print) cout << c << " : " <<  passedBandTest << endl;
		}

		if ( passedBandTest ){
			indexSet(index, photonset, photons_per_particle, particle_index, print);
		}
		if (print) diagnostic_print(index, particle_index, photons, photonset, theta_center_min, theta_center_max, min_photon_time, max_photon_time);
	}

	return;
}