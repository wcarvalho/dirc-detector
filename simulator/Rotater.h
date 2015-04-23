#ifndef _____simulate_____
#define _____simulate_____

#include <iostream>
#include <vector>
#include <cmath>
#include "TVector3.h"
#include "iomanip"


using namespace std;


// class used to rotate a unit vector into the frame given by a provided theta and phi
class Rotater
{
public:
	Rotater() : Zero(0,0,0), X(1,0,0), Y(0,1,0), Z(0,0,1), Direction("Outof") {}
	~Rotater(){}

	void Rotate_Photon(double &pho_theta, double &pho_phi);
	void Feed_Particle(double par_theta, double par_phi);
	void ChangeFrame(){
		Angle *= -1;
		Set_Matrix();
	}

	TVector3 RotationVector;
	TVector3 Zero;
  TVector3 X;
  TVector3 Y;
  TVector3 Z;
	string Direction;
	double Angle;
	double Matrix[3][3];

private:
	void Set_Matrix();
};

#endif