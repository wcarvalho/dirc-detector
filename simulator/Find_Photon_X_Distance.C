#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/functions.h"

using namespace std;

void Find_Photon_X_Distance(Detector d, Photon &photon, string Output)
{
	if(photon.UnitVector.X() >= 0)
	{
		photon.X_Distance = d.Length - photon.X;	
	}
	else
	{
		photon.X_Distance	= -photon.X;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "X_Distance = " << photon.X_Distance << endl;
	}
}