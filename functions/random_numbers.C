#include <iostream>
#include <math.h>
#include "TRandom3.h"
#include "../headers/random.h"


using namespace std;

double* Random::Array(int size)
{
  Rndm();
  double* x = new double[size];
  RndmArray(size,x);
  return x;
}

void Random::Int(int beginning, int end, int &x)
{
	x = beginning + Integer(end-beginning);
}
