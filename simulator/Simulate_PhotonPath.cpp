#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectories for a single Photon of a single Event
================================================================================================*/

void Simulate_PhotonPath(Detector& d, Photon &photon, double smear, bool print)
{
	print = false; // TEMPORARY
	if (print){ TabToLevel(2); cout << "Simulate_PhotonPath:\n"; }
	static Simulate simPho(0, 0);
	simPho.ResetTime();
	simPho.SetAngle(photon.Theta, photon.Phi);
	simPho.SetStart(photon.X, photon.Y, photon.Z);
	simPho.SetDim(d.Length, d.Width, d.Height);
	static double photon_velocity = 1/d.n;
	simPho.SetVelocity(photon_velocity);
	photon.SetVelocity(photon_velocity);

	Random r;
	while ((photon.X != d.Length) && (photon.X != 0))
	{
		simPho.GotoWall(print);
		double &x_p = simPho.coord[0];
		if((x_p == 0 ) || (x_p == d.Length))
		{
			photon.SetAngle(simPho.Theta, simPho.Phi);
			photon.X = simPho.coord[0];
			photon.Y = simPho.coord[1];
			photon.Z = simPho.coord[2];
			photon.Time_Traveled += simPho.GetTimeTraveled();
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