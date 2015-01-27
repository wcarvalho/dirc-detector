#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"

using namespace std;

void Find_Particle_Y_Distance(Detector d, Particle &particle, string Output)
{
	if(particle.UnitVector.Y() >= 0)
	{
		particle.Y_Distance = d.Width - particle.Y;	
	}
	else
	{
		particle.Y_Distance	= -particle.Y;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "Y_Distance = " << particle.Y_Distance << endl;
	}
}