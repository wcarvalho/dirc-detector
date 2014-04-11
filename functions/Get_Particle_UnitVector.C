#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TMath.h"
#include "dirc_objects.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Get Unit Vector
================================================================================================*/

void Get_Particle_UnitVector(Particle &particle)
{
	double theta = particle.Theta;
	double phi = particle.Phi;
	double xunit, yunit, zunit;

	xunit = sin(theta)*cos(phi);
	yunit = sin(theta)*sin(phi);
	zunit = cos(theta);
	TVector3 unit_vector(xunit, yunit, zunit);

	particle.UnitVector = unit_vector;
}