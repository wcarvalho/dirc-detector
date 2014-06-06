#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"

using namespace std;

void Find_Particle_Z_Distance(Detector d, Particle &particle, string Output)
{
	if(particle.UnitVector.Z() >= 0)
	{
		particle.Z_Distance = d.Height - particle.Z;	
	}
	else
	{
		particle.Z_Distance	= -particle.Z;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "Z_Distance = " << particle.Z_Distance << endl;
	}
}