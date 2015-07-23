#include "simulator.h"

void SimulatePhotons(Detector& d, PhotonEvent& photon_event, Particles particles, int& seed, double const& quantum_efficiency, bool print){
	Photons& photons = photon_event.Photons;

	int photons_lost_from_quantum_efficiency;
	int photons_lost_from_critical_angle;

	double total_photons = photons.size();

	for (int i = 0; i < photons.size(); i++)
	{
		auto& photon = photons.at(i);
		Simulate_PhotonPath(d, photon, seed, false);

		if (photon.Flag) ++ photons_lost_from_critical_angle;
		if (photon.Flag == 0){
			photon.Flag = quantum_efficiency_cut(quantum_efficiency, seed, print);
			if (photon.Flag) ++photons_lost_from_quantum_efficiency;
		}

		CheckForFlag(photon_event, i, "no");

		if (photon.Flag){
			particles.at(photon.WhichParticle).nPhotonsPassed -= 1;
		}
	}

	double photons_left = photons.size();

	double percent_photons_lost_from_quantum_efficiency_all = photons_lost_from_quantum_efficiency/total_photons*100;

	double percent_photons_lost_from_quantum_efficiency_detector = photons_lost_from_quantum_efficiency/(total_photons - photons_lost_from_critical_angle)*100;

	double percent_photons_lost_from_critical_angle = photons_lost_from_critical_angle/total_photons*100;

	cout << "\n\t% Photons lost from Critical Angle and Energy Cut = " << percent_photons_lost_from_critical_angle << endl;
	cout << "\t% Photons lost from Quantum Efficiency:\n";
	cout << "\t\tOf all = " << percent_photons_lost_from_quantum_efficiency_all << endl;
	cout << "\t\tOf detected = " << percent_photons_lost_from_quantum_efficiency_detector << endl;
}