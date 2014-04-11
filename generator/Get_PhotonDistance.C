#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Get Distance to nearest wall for photon
================================================================================================*/

void Get_PhotonDistance(Detector d, Photon &photon, string Output)
{
	if (Output == "yes")
	{
		TabToLevel(4); cout << "Get_PhotonDistance:\n";
	}
	Find_Photon_X_Distance(d, photon, Output);
	Find_Photon_Y_Distance(d, photon, Output);
	Find_Photon_Z_Distance(d, photon, Output);
}
