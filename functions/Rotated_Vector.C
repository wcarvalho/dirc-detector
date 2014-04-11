#include <iostream>
#include <vector>
#include <cmath>
#include "../headers/functions.h"
#include "TVector3.h"
using namespace std;

/*================================================
Rotate vector according to rotation matrix
================================================*/

TVector3 Rotated_Vector(vector< vector<double> > R, TVector3 vector, string Output)
{
	Output = "";
  int m, n, p;
	TVector3 vector_prime;
  
  for (m = 0; m < R.size(); m++)
  {
    vector_prime[m] = 0;
    for (p = 0; p < R.size(); p++)
    {
      vector_prime[m] = vector_prime[m] + R[m][p]*vector[p];
    }
  }

  if (Output == "yes"){ TabToLevel(4); cout << "Rotated_Vector = "; Print_TVector(vector_prime); }

  return vector_prime;
}
