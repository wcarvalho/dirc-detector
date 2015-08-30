#include "simulator.h"

void SimulatePhotons(Detector& d, PhotonEvent& photon_event, Particles particles, int& seed, double const& quantum_efficiency, bool print){
	Photons& photons = photon_event.Photons;

	int photons_lost_from_quantum_efficiency = 0;
	int photons_lost_from_critical_angle = 0;

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

	if (print){
		cout << "\n\tPhotons lost from Critical Angle and Energy Cut: " << photons_lost_from_critical_angle << " (" << percent_photons_lost_from_critical_angle << " percent)\n";

		cout << "\tPhotons lost from Quantum Efficiency: " << photons_lost_from_quantum_efficiency << endl;

		cout << "\t\tOf all = " << percent_photons_lost_from_quantum_efficiency_all << " percent\n";
		cout << "\t\tOf detected = " << percent_photons_lost_from_quantum_efficiency_detector << " percent\n";
	}
}