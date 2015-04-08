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
Reconstruct one "Event" in the frame of one of the "tracks"
================================================================================================*/

void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, vector<PhotonOut> photons, bool print)
{

	int i;

	double *pho_theta, *pho_phi;
	reconstruction.Photons.push_back(photons);														// store photons so original photons aren't manipulated, basically resets the photons every time this function is called
	// cout << "# of reconstructed photons = " << reconstruction.Photons.back().size() << endl;

	static Rotater r;
	r.Direction = "Into";
	r.Feed_Particle(particle.Theta, particle.Phi);
	// cout << "particle.Theta, particle.Phi = " << particle.Theta << ", " << particle.Phi << endl;
	for(i = 0; i < reconstruction.Photons.back().size(); i++)
	{
		pho_theta = &reconstruction.Photons.back()[i].Theta;
		if (print) cout << i << "\ttheta = " << *pho_theta << " -> ";
		pho_phi = &reconstruction.Photons.back()[i].Phi;
		r.Rotate_Photon(*pho_theta, *pho_phi);
		if (print) cout << "\ttheta = " << *pho_theta << endl;
	}

}
