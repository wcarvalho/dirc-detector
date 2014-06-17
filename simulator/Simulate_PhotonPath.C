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

void Simulate_PhotonPath(Detector d, Photon &photon)
{
	Displayer Output;
	if (Output.Main == "yes")
	{
		TabToLevel(3); cout << "Simulate_PhotonPath:\n";
	}
	Random r;
	while ((photon.X != d.Length) && (photon.X != 0))
	{
		Get_PhotonDistance(d, photon, Output.Trivial);
		Get_PhotonWall(photon, Output.Trivial);
		Move_Photon(photon, Output.Trivial);
		if ((photon.X == d.Length) || (photon.X == 0))
		{
			photon.Theta += r.Gaus(0., .01);
			photon.Phi += r.Gaus(0., .01);
			
			if (Output.Important == "yes"){
				TabToLevel(4); cout << "Simulation for Photon "<< photon.Which <<" is Complete. It took " << photon.Reflections << " Reflections.\n";
			}
			return;
		}
		CheckAngel(d, photon, Output.Trivial);
		if (photon.Flag == 1){ 
			return; }
		Change_Angle(photon, Output.Trivial);
		photon.Reflections++;
		// if (photon.Reflections > 800)
		// {
		// 	photon.Flag = 1;
		// 	return;
		// }
	}
}