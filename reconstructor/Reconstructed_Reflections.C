#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "../headers/reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct possible reflections of Photon
Takes in some vector of photons (that may be empty) and puts reconstructions of photon with theta and phi in it
================================================================================================*/

void Reconstructed_Reflections(vector<PhotonOut> &pho, double theta, double phi, Displayer disp)
{
	Simulate sim(theta, phi);

	if (disp.Trivial == "yes")
	{
		TabToLevel(2); cout << "Reconstructed_Reflections: Begins\n";
		disp.Angle(theta, phi);
	}
		sim.FlipY();
		PhotonOut y(sim.Theta, sim.Phi);
		pho.push_back(y);

		sim.FlipZ();
		PhotonOut yz(sim.Theta, sim.Phi);
		pho.push_back(yz);
	
		sim.FlipY();
		PhotonOut z(sim.Theta, sim.Phi);
		pho.push_back(z);

	// if (sim.Vec.Z() < 0)
	// {
		// XY_Reflections(pho, sim, disp);
		if (disp.Trivial == "yes"){ TabToLevel(3); cout << "Flipped Z\n" ;disp.Angle(z.Theta, z.Phi); }
		// XY_Reflections(pho, sim, disp);
	// }
	// else{ XY_Reflections(pho, sim, disp); }
	if (disp.Trivial == "yes")
	{
		TabToLevel(3); cout << pho.size() << " Reconstructed reflections\n";
	}
}

void XY_Reflections(vector<PhotonOut> &pho, Simulate sim, Displayer disp)
{
	sim.FlipX();
	PhotonOut x(sim.Theta, sim.Phi);
	pho.push_back(x);
	if (disp.Trivial == "yes"){ disp.Angle(x.Theta, x.Phi); }

	sim.FlipY();
	PhotonOut xy(sim.Theta, sim.Phi);
	pho.push_back(xy);
	if (disp.Trivial == "yes"){ disp.Angle(xy.Theta, xy.Phi); }

	sim.FlipX();
	PhotonOut y(sim.Theta, sim.Phi);
	pho.push_back(y);
	if (disp.Trivial == "yes"){ disp.Angle(y.Theta, y.Phi); }

}	
