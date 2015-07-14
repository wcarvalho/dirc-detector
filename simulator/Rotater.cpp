#include "Rotater.h"

void Rotater::Rotate_Photon(double &pho_theta, double &pho_phi)
{
	static int m, n, p;
	static double pho_x, pho_y, pho_z;

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