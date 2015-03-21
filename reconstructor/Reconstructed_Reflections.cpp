#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Reconstruct possible reflections of Photon
Takes in some vector of photons (that may be empty) and puts reconstructions of photon with theta and phi in it
================================================================================================*/

void Reconstructed_Reflections(vector<PhotonOut> &pho, double theta, double phi, bool print)
{
	Simulate sim(theta, phi);

		sim.FlipY();
		PhotonOut y(sim.Theta, sim.Phi);
		pho.push_back(y);
		// printf("\t\ttheta = %f, phi = %f\n", sim.Theta, sim.Phi);
		sim.FlipZ();
		PhotonOut yz(sim.Theta, sim.Phi);
		pho.push_back(yz);
		// printf("\t\ttheta = %f, phi = %f\n", sim.Theta, sim.Phi);
		sim.FlipY();
		PhotonOut z(sim.Theta, sim.Phi);
		pho.push_back(z);
		// printf("\t\ttheta = %f, phi = %f\n", sim.Theta, sim.Phi);
		// cout << "\n";
}

// void XY_Reflections(vector<PhotonOut> &pho, Simulate sim, Displayer disp)
// {
// 	sim.FlipX();
// 	PhotonOut x(sim.Theta, sim.Phi);
// 	pho.push_back(x);
// 	if (disp.Trivial == "yes"){ disp.Angle(x.Theta, x.Phi); }

// 	sim.FlipY();
// 	PhotonOut xy(sim.Theta, sim.Phi);
// 	pho.push_back(xy);
// 	if (disp.Trivial == "yes"){ disp.Angle(xy.Theta, xy.Phi); }

// 	sim.FlipX();
// 	PhotonOut y(sim.Theta, sim.Phi);
// 	pho.push_back(y);
// 	if (disp.Trivial == "yes"){ disp.Angle(y.Theta, y.Phi); }

// }
