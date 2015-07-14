#include <iostream>
#include <vector>
#include <cmath>
#include "functions.h"
#include "TVector3.h"
using namespace std;

/*================================================
Get the rotation vector for this particle
================================================*/

// TVector3 Rotation_Vector(Particle particle, string Output)
// {
// 	TVector3 Rotation_Vector(0,0,0);

//   if (particle.UnitVector[2] != 1)
// 	{
// 		Rotation_Vector = Z.Cross(particle.UnitVector);
// 		if (Output == "yes") { TabToLevel(3); cout << "Rotation_Vector = "; Print_TVector(Rotation_Vector); }
// 	}
// 	else
// 		{
// 			if (Output == "yes"){ TabToLevel(3); cout << "Already in correct frame\n"; }
// 		}
// 	return Rotation_Vector;
// }
