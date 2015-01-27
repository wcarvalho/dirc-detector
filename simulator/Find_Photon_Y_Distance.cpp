#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"

using namespace std;

void Find_Photon_Y_Distance(Detector d, Photon &photon, string Output)
{
	if(photon.UnitVector.Y() >= 0)
	{
		photon.Y_Distance = d.Width - photon.Y;	
	}
	else
	{
		photon.Y_Distance	= -photon.Y;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "Y_Distance = " << photon.Y_Distance << endl;
	}
}