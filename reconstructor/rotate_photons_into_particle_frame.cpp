#include "Rotater.h"
#include "reconstructor.h"


Photons rotate_photons_into_particle_frame(double const theta, double const phi, Photons const& original_photons){

	double pi=3.14159265358979312;
	Photons rotated_photons = original_photons;

	static Rotater r; r.Feed_Particle(theta, phi);
	r.ChangeFrame();

	for (auto& photon: rotated_photons){
		r.Rotate_Photon(photon.Theta, photon.Phi);
	}

	return std::move(rotated_photons);

}