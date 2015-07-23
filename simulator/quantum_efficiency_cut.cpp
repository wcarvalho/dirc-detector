#include "TRandom3.h"
#include <iostream>
// if random number thrown is below quantum_efficiency, then function returns false, meaning the photon is kept. If it is above, the functions returns true, meaning the photon is cut.
bool quantum_efficiency_cut(double const& quantum_efficiency, int& seed, bool print){
	if(seed) ++seed;

	static TRandom3 r; r.SetSeed(seed);
	static int random_integer; random_integer = r.Integer(100);
	// std::cout << "\trandom_integer = " << random_integer << std::endl;
	// std::cout << "\tquantum_efficiency = " << quantum_efficiency << std::endl;
	bool remove_photon = true;
	if (random_integer < quantum_efficiency)
		remove_photon = false;

	return remove_photon;
}