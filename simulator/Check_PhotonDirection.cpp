#include <iostream>
#include <string>
#include <math.h>
#include "TVector3.h"
#include "dirc_objects.h"
#include "simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectories for a single Photon of a single "Event"
================================================================================================*/

void Check_PhotonDirection(double v, TVector3 &normal_vector)
{
	if(v<0)
	{
		normal_vector = -normal_vector;
	}
}
