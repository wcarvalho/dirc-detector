#include <iostream>
#include "TVector3.h"

using namespace std;
/*================================================================================================
Push Back new Event (with parameters) into photon_events and particle_events
================================================================================================*/

void Print_TVector(TVector3 v)
{
	cout << v.X() << "\t" << v.Y() << "\t" << v.Z() << endl;
}