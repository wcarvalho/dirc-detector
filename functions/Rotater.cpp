#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/functions.h"
#include "TVector3.h"
#include "iomanip"


using namespace std;

/*================================================================================================
All functions pertinent to Rotater class
================================================================================================*/

void Rotater::Rotate_Photon(double &pho_theta, double &pho_phi)
{
	int m, n, p;
	double pho_x, pho_y, pho_z;
	
	pho_x = sin(pho_theta)*cos(pho_phi);
	pho_y = sin(pho_theta)*sin(pho_phi);
	pho_z = cos(pho_theta);

	TVector3 pho(pho_x, pho_y, pho_z);
	TVector3 new_pho(0,0,0);

  for (m = 0; m < 3; m++)
  {
    for (p = 0; p < 3; p++)
    {
      new_pho[m] = new_pho[m] + Matrix[m][p]*pho[p];
    }
  }

  pho_theta = new_pho.Theta();
  pho_phi = new_pho.Phi();

}

void Rotater::Feed_Particle(double par_theta, double par_phi)
{
	double par_x, par_y, par_z;

	par_x = sin(par_theta)*cos(par_phi);
	par_y = sin(par_theta)*sin(par_phi);
	par_z = cos(par_theta);

	TVector3 par(par_x, par_y, par_z);

	RotationVector = Z.Cross(par);
	if (RotationVector != Zero){ RotationVector.SetMag(1.); }
	Angle = Z.Angle(par);
	if (Direction == "Into") { Angle = -Angle; }
	Set_Matrix();

}

void Rotater::Set_Matrix()
{
	int i, j;
  double x,y,z;
  double nu, s, c;

  x = RotationVector[0];
  y = RotationVector[1];
  z = RotationVector[2];
  nu = 1.0-cos(Angle);
  s = sin(Angle);
  c = cos(Angle);

  Matrix[0][0] = c + x*x*nu;
  Matrix[0][1] = x*y*nu - z*s;
  Matrix[0][2] = x*z*nu + y*s;
  Matrix[1][0] = y*x*nu + z*s;
  Matrix[1][1] = c + y*y*nu;
  Matrix[1][2] = y*z*nu - x*s;
  Matrix[2][0] = z*z*nu - y*s;
  Matrix[2][1] = z*y*nu + x*s;
  Matrix[2][2] = c + z*z*nu;
}





























/*
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

	Displayer d;

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

  if (Output != "no")
  {
  	TabToLevel(3); cout << "Rotation Matrix =\n";
  	// d.Array_2D(matrix, 3, 3);
  	// Print_2D_Vector((vector< vector<double> >) matrix);
  	for (i = 0; i < 3; i++)
  	{
	  	TabToLevel(4);
  		for (j=0;j <3; j++)
  		{
  			cout << setw(13) << Matrix[i][j] << "|";
  		}
  		cout << endl;
  	}
  }

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
*/