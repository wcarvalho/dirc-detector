#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Get X number of random emission times for particle
================================================================================================*/

double *Random_Emission_Times(Particle particle, string type, string Output)
{
	int i;
	double *x = random_numbers(particle.Emissions, 4 + particle.Which);

	if (Output == "yes"){
		TabToLevel(4); cout << "Random_Emission_Times:\n";
	}

	for (i = 0; i < particle.Emissions; i++)
	{
		if (type == "random")
		{
			x[i] = x[i]*particle.Time;
		}
		if (type == "zero")
		{
			x[i] = 0;
		}
		if (Output == "yes")
		{
		TabToLevel(5); cout << "Emission Time " << i << " = " << x[i] << endl;
		}
	}

	return x;
}