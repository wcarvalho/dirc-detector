#include <iostream>
#include <vector>
#include <string>
#include <cmath>
#include "TMath.h"
#include "dirc_objects.h"
#include "functions.h"


using namespace std;

/*================================================================================================
Get Unit Vector
================================================================================================*/

TVector3 Get_UnitVector(double theta, double phi)
{
	double xunit, yunit, zunit;

	xunit = sin(theta)*cos(phi);
	yunit = sin(theta)*sin(phi);
	zunit = cos(theta);
	TVector3 unit_vector(xunit, yunit, zunit);

	return unit_vector;
}