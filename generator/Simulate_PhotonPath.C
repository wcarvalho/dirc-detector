#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectories for a single Photon of a single "Event"
================================================================================================*/

void Simulate_PhotonPath(Detector d, Photon &photon, Particle &particle, string Output)
{
	if (Output == "yes")
	{
		TabToLevel(3); cout << "Simulate_PhotonPath:\n";
	}
	while (photon.X != d.Length)
	{
		Get_PhotonDistance(d, photon, Output);
		Get_PhotonWall(photon, Output);
		CheckAngel(d, photon, Output);
		if (photon.Flag == 1){ return; }
		Move_Photon(photon, Output);
		if (photon.X == d.Length)
		{
			// SmearAngle();
			if (Output == "yes"){
				TabToLevel(2); cout << "Simulation for Photon "<< photon.Which <<" is Complete. It took " << photon.Reflections << " Reflections.\n";
			}
			return;
		}
		Change_Angle(photon, Output);
		photon.Reflections++;
		if (photon.Reflections > 100)
		{
			photon.Flag = 1;
			// TabToLevel(2); cout << "Simulation for Photon "<< photon.Which <<" is taking too long\n";
			// Output = "yes";
		}
	}
}