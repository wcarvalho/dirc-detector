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

void Get_Photon_UnitVector(Photon &photon)
{
	double theta = photon.Theta;
	double phi = photon.Phi;
	double xunit, yunit, zunit;

	xunit = sin(theta)*cos(phi);
	yunit = sin(theta)*sin(phi);
	zunit = cos(theta);
	TVector3 unit_vector(xunit, yunit, zunit);

	photon.UnitVector = unit_vector;
}