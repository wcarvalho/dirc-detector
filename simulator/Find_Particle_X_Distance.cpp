#include <iostream>
#include <string>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/functions.h"

using namespace std;

void Find_Particle_X_Distance(Detector d, Particle &particle, string Output)
{
	if(particle.UnitVector.X() >= 0)
	{
		particle.X_Distance = d.Length - particle.X;	
	}
	else
	{
		particle.X_Distance	= -particle.X;
	}
	if (Output == "yes"){
		TabToLevel(5); cout << "X_Distance = " << particle.X_Distance << endl;
	}
}