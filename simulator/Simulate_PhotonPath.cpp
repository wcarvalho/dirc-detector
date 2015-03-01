#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectories for a single Photon of a single Event
================================================================================================*/

void Simulate_PhotonPath(Detector d, Photon &photon, double smear, bool print)
{
	print = false; // TEMPORARY
	if (print){ TabToLevel(2); cout << "Simulate_PhotonPath:\n"; }
	Simulate simPho(photon.Theta, photon.Phi);
	simPho.SetStart(photon.X, photon.Y, photon.Z);
	simPho.SetDim(d.Length, d.Width, d.Height);

	Random r;
	while ((photon.X != d.Length) && (photon.X != 0))
	{
		simPho.GotoWall(print);
		double &x_p = simPho.coord[0];
		// Get_PhotonDistance(d, photon, Output.Trivial);
		// Get_PhotonWall(photon, Output.Trivial);
		// Move_Photon(photon, Output.Trivial);
		if((x_p == 0 ) || (x_p == d.Length))
		{
			// photon.Theta += r.Gaus(0., smear);
			// photon.Phi += r.Gaus(0., smear);
			double th = simPho.Theta;
			double ph = simPho.Phi;
			Photon pho_temp(th, ph);
			photon = pho_temp;
			photon.Time_Traveled = simPho.time_traveled;
			return;
		}

		double th = simPho.Theta;
		double ph = simPho.Phi;
		Photon pho_temp(th, ph);
		pho_temp.Wall = simPho.wall;
		CheckAngel(d, pho_temp, "no");
		photon.Flag = pho_temp.Flag;
		if (photon.Flag == 1){
			return;
		}
		// Change_Angle(photon, Output.Trivial);
		simPho.Reflect(print);
		photon.Reflections++;
		// if (photon.Reflections > 800)
		// {
		// 	photon.Flag = 1;
		// 	return;
		// }
	}
}