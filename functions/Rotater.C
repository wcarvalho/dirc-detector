#include <iostream>
#include <vector>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "TVector3.h"


using namespace std;

/*================================================================================================
All functions pertinent to Rotater class
================================================================================================*/

TVector3 Rotater::Rotation_Vector(Particle particle, string Output)
{
	TVector3 Rotation_Vector(0,0,0);
	Rotater r;
	TVector3 Z = r.Z_UnitVector;
  if (particle.UnitVector[2] != 1)
	{
		Rotation_Vector = Z.Cross(particle.UnitVector);
		if (Output == "yes") { TabToLevel(3); cout << "Rotation_Vector = "; Print_TVector(Rotation_Vector); }
	}
	else 
		{
			if (Output == "yes"){ TabToLevel(3); cout << "Already in correct frame\n"; }
		}
	return Rotation_Vector;
}

void Rotater::RotationMatrix(double matrix[3][3], TVector3 Tvector, double angle, string Output)
{
  int i, j;
  double X,Y,Z;
  double nu, s, c;

  X = Tvector[0];
  Y = Tvector[1];
  Z = Tvector[2];
  nu = 1-cos(angle);
  s = sin(angle);
  c = cos(angle);

  matrix[0][0] = c + X*X*nu;
  matrix[0][1] = X*Y*nu - Z*s;
  matrix[0][2] = X*Z*nu + Y*s;
  matrix[1][0] = Y*X*nu + Z*s;
  matrix[1][1] = c + Y*Y*nu;
  matrix[1][2] = Y*Z*nu - X*s;
  matrix[2][0] = Z*X*nu - Y*s;
  matrix[2][1] = Z*Y*nu + X*s;
  matrix[2][2] = c + Z*Z*nu;

}

void Rotater::Rotate_Photons(Rotater r, vector<Photon> &photons, string Output)
{

	int i;
	for ( i = 0; i < photons.size(); i++)
	{
		r.Rotate_Photon(r, photons[i], Output);
	}

}

void Rotater::Rotate_Photon(Rotater r, Photon &photon, string Output)
{
	photon.UnitVector = Rotated_Vector(r.Matrix, photon.UnitVector, Output);
	Update_Photon_ThetaPhi(photon, Output);
}

TVector3 Rotater::Rotated_Vector(double R[3][3], TVector3 vector, string Output)
{
	Output = "";
  int m, n, p;
	TVector3 vector_prime;
  
  for (m = 0; m < 3; m++)
  {
    vector_prime[m] = 0;
    for (p = 0; p < 3; p++)
    {
      vector_prime[m] = vector_prime[m] + R[m][p]*vector[p];
    }
  }

  if (Output == "yes"){ TabToLevel(4); cout << "Rotated_Vector = "; Print_TVector(vector_prime); }

  return vector_prime;
}
