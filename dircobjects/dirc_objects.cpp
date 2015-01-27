#include "dirc_objects.h"
#include <iostream>
#include <cmath>
#include <string>
#include "TVector3.h"
#include "TMath.h"
#include "particle_objects.h"
#include "photon_objects.h"

using namespace std;

// void sEvent::NewParticle()
// {
// 	Particle_new p;
// 	sParticle sp;

// 	Par.push_back(p);
// 	sPar.push_back(sp);

// }

// void sEvent::NewPhoton()
// {
// 	Photon_new p;
// 	sPhoton sp;

// 	Pho.push_back(p);
// 	sPho.push_back(sp);

// }

//======================================================================
//____________Check if Angle passes critical angle test_________________
void Detector::CheckAngle(double theta, double phi, int Wall, int &Flag)
{
	double pi = TMath::Pi();
	double angle;
	double dotproduct, magnitudeproduct;
  TVector3 normal_vector;
  TVector3 UnitVector(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
  
  vector<TVector3> normal;
  normal.clear(); normal.push_back(x_normal); normal.push_back(y_normal); normal.push_back(z_normal);

  for (unsigned int i = 0; i<3; i++)
  {
  	if (Wall == i+1)
		{
			normal_vector = normal.at(i);
			if(UnitVector.Mag() < 0){ normal_vector = -normal_vector; }
		}
  }

  dotproduct = UnitVector.Dot(normal_vector);
  magnitudeproduct = UnitVector.Mag()*normal_vector.Mag();
  angle = acos (dotproduct / magnitudeproduct);
  if ((angle < CriticalAngle) || (angle > (pi - CriticalAngle)&&(angle < pi)))
  {
    Flag = 1;
  }
  if ( fabs(UnitVector.X()) <.005)
  {
    Flag = 1;
  }
}