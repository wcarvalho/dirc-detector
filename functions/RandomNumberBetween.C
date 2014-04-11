#include "TRandom3.h"
using namespace std;

/*================================================
get random number between two values
================================================*/

double RandomNumberBetween(double beginning, double end, int input_number)
{
  TRandom3 r;
  double* x = new double[1];
  double number;
  double difference = end - beginning;

  r.SetSeed(0);
  r.Rndm();
  r.RndmArray(1,x);

  number = beginning + difference*x[0];

  return number;
}