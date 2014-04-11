#include "TRandom3.h"
using namespace std;

/*================================================
Generate an array with n random numbers
================================================*/

double* random_numbers(int size, int input_number)
{
  TRandom3 r;
  r.SetSeed(0);
  r.Rndm();
  double* x = new double[size];
  r.RndmArray(size,x);
  return x;
}