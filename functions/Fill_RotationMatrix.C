#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/functions.h"
#include "TVector3.h"
using namespace std;

/*================================================
create a rotation matrix
================================================*/

void Fill_RotationMatrix(vector< vector<double> > &matrix, TVector3 Tvector, double angle, string Output)
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

  matrix.at(0)[0] = c + X*X*nu;
  matrix.at(0)[1] = X*Y*nu - Z*s;
  matrix.at(0)[2] = X*Z*nu + Y*s;
  matrix.at(1)[0] = Y*X*nu + Z*s;
  matrix.at(1)[1] = c + Y*Y*nu;
  matrix.at(1)[2] = Y*Z*nu - X*s;
  matrix.at(2)[0] = Z*X*nu - Y*s;
  matrix.at(2)[1] = Z*Y*nu + X*s;
  matrix.at(2)[2] = c + Z*Z*nu;

  if (Output == "yes")
  {
  	TabToLevel(3); cout << "RotationMatrix =\n";
  	Print_2D_Vector(matrix);
  }
}