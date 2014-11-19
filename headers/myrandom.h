#ifndef wilkasrandom
#define wilkasrandom

#include <iostream>
#include "TMath.h"
#include "TRandom3.h"

using namespace std;

class Random : public TRandom3
{
public:
	Random(double in = 0.) : Input(in) { SetSeed(in); }
	~Random(){}

	double* Array(int size);
	void Int(int beginning, int end, int &x);
	
	double Input;
};

#endif