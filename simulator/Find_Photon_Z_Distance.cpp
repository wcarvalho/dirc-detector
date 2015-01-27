#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"

using namespace std;

void Find_Photon_Z_Distance(Detector d, Photon &photon, string Output)
{
	if(photon.UnitVector.Z() >= 0)
	{
		photon.Z_Distance = d.Height - photon.Z;	
	}
	else
	{
		photon.Z_Distance	= -photon.Z;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "Z_Distance = " << photon.Z_Distance << endl;
	}
}