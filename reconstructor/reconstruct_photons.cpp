#include "reconstructor.h"
#include "Simulate.h"

Photons reflect_photon(PhotonOut const& photon);

Photons reconstruct_photons(Photons const& set){

	unsigned setsize = set.size();

	Photons newset = set;
	for(unsigned i = 0; i < setsize; ++i){
		auto& photon = newset.at(i);
		auto reflected_photons = reflect_photon(photon);
		for (auto& photon: reflected_photons)
			newset.push_back(std::move(photon));
	}

	return std::move(newset);
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