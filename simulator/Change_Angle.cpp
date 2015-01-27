#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Move Photon to Wall
================================================================================================*/

void Change_Angle(Photon &photon, string Output)
{
	if (Output == "yes")
	{
		TabToLevel(4); cout << "Change_Angle:\n";
		TabToLevel(5); Print_TVector(photon.UnitVector);
		// cout << "photon.Wall = " << photon.Wall << endl;
	}
	if (photon.Wall == 1)
	{
		photon.UnitVector.SetX(-photon.UnitVector.X());
	}
	if (photon.Wall == 2)
	{
		photon.UnitVector.SetY(-photon.UnitVector.Y());
	}
	if (photon.Wall == 3)
	{
		photon.UnitVector.SetZ(-photon.UnitVector.Z());
	}


	if (Output == "yes")
	{
		TabToLevel(4); cout << "->\t"; Print_TVector(photon.UnitVector);
	}
	Update_Photon_ThetaPhi(photon, Output);


	// photon.Phi = photon.UnitVector.Phi();

}