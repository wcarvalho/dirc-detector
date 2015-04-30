#include "reconstructor.h"

string histogram_name(int const& event, int const& particle);
string histogram_title(int const& event, int const& particle);

TH2D histogram_photon_angles(int const& event, int const& particle, vector<PhotonOut> const& photons){

	double pi = TMath::Pi();

	string title = histogram_title(event, particle);
	string name = histogram_name(event, particle);

	TH2D histogram(title.c_str(), name.c_str(), 2000, -pi, pi, 1000, 0, pi);

	if ( photons.empty() )
		return std::move(histogram);

	histogram.SetDefaultSumw2();

	for (auto& photon: photons){
		double theta = photon.Theta;
		double phi = photon.Phi;
		histogram.Fill(phi, theta);
	}
	return std::move(histogram);
}


string histogram_name(int const& event, int const& particle){
	static stringstream ss; ss.str("");
	ss << "Event_" << setfill('0') <<setw(3) << event << "_Particle_" << particle;
  return ss.str();
}

string histogram_title(int const& event, int const& particle){
	static stringstream ss; ss.str("");
	ss << "Event " << event << ", Particle " << particle;
  return ss.str();
}