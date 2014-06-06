#include "dirc_objects.h"
#include "TMath.h"
#include "../headers/functions.h"
#include "../headers/simulator.h"
#include "../headers/Simulate.h"

void Release_Photons(Simulate simPar, PhotonEvent &event, int released, double EmissionAngle)
{
	vector<Photon> &Phos = event.Photons;
	int end = event.iterator + released;

	Random r;
	for (int pho = event.iterator; pho < end; pho++)
	{
		Photon P(EmissionAngle, r.Uniform(0,2*TMath::Pi()));
		P.X = simPar.coord[0];
		P.Y = simPar.coord[1];
		P.Z = simPar.coord[2];
		Phos.push_back(P);
	}
	
}