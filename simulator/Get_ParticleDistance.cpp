#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Get Distance to nearest wall for particle
================================================================================================*/

void Get_ParticleDistance(Detector d, Particle &particle, string Output)
{
	if (Output == "yes"){
		TabToLevel(4); cout << "Get_ParticleDistance:\n";
	}
	Find_Particle_X_Distance(d, particle, Output);
	Find_Particle_Y_Distance(d, particle, Output);
	Find_Particle_Z_Distance(d, particle, Output);
}
