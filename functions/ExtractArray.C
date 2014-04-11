#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "TH2D.h"
#include "../headers/reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Take Thetas or Phis from photon data
================================================================================================*/

double *ExtractPhi(vector<Photon> v, string Output)
{
	int i;
	double *x = new double[v.size()];

	for (i = 0; i < v.size(); i++)
	{
		x[i] = v[i].Phi;
	}
	return x;
}

double *ExtractTheta(vector<Photon> v, string Output)
{
	int i;
	double *y = new double[v.size()];

	for (i = 0; i < v.size(); i++)
	{
		y[i] = v[i].Theta;
	}
	return y;
}