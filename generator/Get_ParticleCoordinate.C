#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectory for a single Particle of a single "Event"
================================================================================================*/

void Get_ParticleCoordinate(double t, Particle &particle, string Output)
{
	particle.X = particle.X + particle.UnitVector.X()*t; 
	particle.Y = particle.Y + particle.UnitVector.Y()*t; 
	particle.Z = particle.Z + particle.UnitVector.Z()*t;
	if (Output == "yes")
	{
		TabToLevel(6); cout << "Particle X = " << particle.X << endl;
		TabToLevel(6); cout << "Particle Y = " << particle.Y << endl;
		TabToLevel(6); cout << "Particle Z = " << particle.Z << endl;
	}

}