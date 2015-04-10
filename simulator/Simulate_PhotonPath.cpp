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
	if (print){ TabToLevel(2); cout << "Simulate_PhotonPath:\n"; }
	static Simulate simPho(0, 0);
	simPho.SetAngle(photon.Theta, photon.Phi);
	simPho.SetStart(photon.X, photon.Y, photon.Z);
	simPho.SetDim(d.Length, d.Width, d.Height);

	while ((photon.X != d.Length) && (photon.X != 0))
	{
		simPho.GotoWall(print);
		double &x_p = simPho.coord[0];
		if((x_p == 0 ) || (x_p == d.Length))
		{
			photon.SetAngle(simPho.Theta, simPho.Phi);
			photon.Time_Traveled = simPho.time_traveled;
			return;
		}
		static Photon pho_temp(0,0);
		pho_temp.SetAngle(simPho.Theta, simPho.Phi);
		pho_temp.Wall = simPho.wall;
		static string input;
		if (print) input = "yes"; else input = "no";
		CheckAngel(d, pho_temp, input);
		photon.Flag = pho_temp.Flag;
		if (photon.Flag == 1){
			return;
		}

		simPho.Reflect(print);
		photon.Reflections++;
	}
}