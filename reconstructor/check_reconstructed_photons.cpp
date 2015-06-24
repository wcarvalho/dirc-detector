#include "reconstructor.h"

void check_reconstructed_photons(Photons& photons){

	unsigned loss = 0;

	for (unsigned i = 0; i < photons.size(); ++i){
		// cout << i << endl;
		auto& photon = photons.at(i);

		static PhotonOut::DircWall calculated_photon_wall;
		calculated_photon_wall = photon.WallFromPhi(photon.Phi);

		static PhotonOut::DircWall actual_photon_wall;
		actual_photon_wall = photon.GetWall();

		// cout << "actual_photon_wall = " << actual_photon_wall << endl;
		// cout << "calculated_photon_wall = " << calculated_photon_wall << endl;
		if (actual_photon_wall != calculated_photon_wall){
			photons.erase(photons.begin()+i);
			--i;
			++loss;
		}
	}

	// cout << "\tloss = " << loss << endl;

}