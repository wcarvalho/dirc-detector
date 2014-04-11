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
Reconstruct one "Event" in the frame of one of the "tracks"
================================================================================================*/

void ReconstructTrack(Reconstruction &reconstruction, ParticleOut particle, vector<PhotonOut> photons, TH2D& h1, string Output)
{
	
	if (Output == "yes")
	{
		TabToLevel(2); cout << "Reconstructing Track\n";
	}
	reconstruction.Photons.push_back(photons);														// store photons so original photons aren't manipulated, basically resets the photons every time this function is called

	Detector d(10,1,1);
	// cout << "\t\ta photon Theta pre is " << reconstruction.Photons.back()[20].Theta << endl;
	Rotate_Photons_IntoFrame(d, particle, reconstruction.Photons.back(), 0, reconstruction.Photons.back().size(), Output);
	// cout << "\t\ta photon Theta post is " << reconstruction.Photons.back()[20].Theta << endl;

	// CreateHistogram(h1, reconstruction.Photons, Output);
	// reconstruction.Histograms.push_back(h1);										// store the histogram from this track
	// h1.Draw("colz");
	// reconstruction.Track.back().Event
	// reconstruction.Track.back().Particle
	// reconstruction.Track.back().EmissionAngle


}
