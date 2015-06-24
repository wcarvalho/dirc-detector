#include "reconstructor.h"
#include "Simulate.h"

double pi = 3.14159265358979312;
Photons reflect_photon(PhotonOut const& photon);
void check_reconstructed_photons(Photons& photons);

Photons reconstruct_photons(Photons const& set){

	unsigned setsize = set.size();

	Photons newset = set;
	for(unsigned i = 0; i < setsize; ++i){
		auto& photon = newset.at(i);
		auto reflected_photons = reflect_photon(photon);
		for (auto& photon: reflected_photons)
			newset.push_back(std::move(photon));
	}

	check_reconstructed_photons(newset);

	return std::move(newset);
}

void check_reconstructed_photons(Photons& photons){

	for (unsigned i = 0; i < photons.size(); ++i){
		cout << i << endl;
		auto& photon = photons.at(i);


		static Photon::DircWall calculated_photon_wall;
		double& photon_phi = photon.Phi;
		static int reflected; reflected = 0;

		cout << "reflected: " << reflected << endl; ++reflected;
		if ((photon_phi > pi/2.) && (photon_phi < 3.*pi/2))
			calculated_photon_wall = Photon::BACK;
		else
			calculated_photon_wall = Photon::FRONT;

		Photon::DircWall actual_photon_wall = photon.GetWall();
		cout << "actual_photon_wall = " << actual_photon_wall << endl;
		cout << "calculated_photon_wall = " << calculated_photon_wall << endl;
		// if (actual_photon_wall != calculated_photon_wall) continue;
	}

}

Photons reflect_photon(PhotonOut const& photon){

	Photons reflected_photons {photon, photon, photon};

	PhotonOut& copy1 = reflected_photons.at(0);
	PhotonOut& copy2 = reflected_photons.at(1);
	PhotonOut& copy3 = reflected_photons.at(2);


	static Simulate simpho(0.,0.);
	simpho.SetAngle(photon.Theta, photon.Phi);
	simpho.FlipY(); copy1.SetAngle(simpho.Theta, simpho.Phi);
	simpho.FlipZ();	copy2.SetAngle(simpho.Theta, simpho.Phi);
	simpho.FlipY(); copy3.SetAngle(simpho.Theta, simpho.Phi);

	return std::move(reflected_photons);
}