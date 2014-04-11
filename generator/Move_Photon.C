#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Move Photon to Wall
================================================================================================*/

void Move_Photon(Photon &photon, string Output)
{
	photon.X = photon.X + photon.UnitVector.X()*photon.Time;
	photon.Y = photon.Y + photon.UnitVector.Y()*photon.Time;
	photon.Z = photon.Z + photon.UnitVector.Z()*photon.Time;
	photon.Time_Traveled = photon.Time_Traveled + photon.Time;

	photon.Distance = sqrt(
			photon.UnitVector.X()*photon.Time*photon.UnitVector.X()*photon.Time+
	    photon.UnitVector.Y()*photon.Time*photon.UnitVector.Y()*photon.Time+                  
	    photon.UnitVector.Z()*photon.Time*photon.UnitVector.Z()*photon.Time
	                       );
	photon.Distance_Traveled = photon.Distance_Traveled + photon.Distance;
	if (Output == "yes")
	{
		TabToLevel(4); cout << "Move_Photon:\n";
		TabToLevel(5); cout << "photon.X = " << photon.X << endl;
		TabToLevel(5); cout << "photon.Y = " << photon.Y << endl;
		TabToLevel(5); cout << "photon.Z = " << photon.Z << endl;
		TabToLevel(5); cout << "photon.Time_Traveled = " << photon.Time_Traveled << endl;
		TabToLevel(5); cout << "photon.Distance_Traveled = " << photon.Distance_Traveled << endl;
	}
}