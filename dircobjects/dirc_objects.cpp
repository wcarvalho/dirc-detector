#include "dirc_objects.h"
#include <iostream>
#include <cmath>
#include <string>
#include "TVector3.h"
#include "TMath.h"
#include "particle_objects.h"
#include "photon_objects.h"

using namespace std;

template <typename T>
int sign(T val){
  if (val < 0) return -1;
  else return 1;
}

//____________Check if Angle passes critical angle test_________________
void Detector::CheckAngle(double const theta, double const phi, int const Wall, int &Flag, bool print)
{
	static double pi = TMath::Pi();
	static double angle;
	static double dotproduct, magnitudeproduct;
  static TVector3 normal_vector;
  TVector3 UnitVector(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));

  if ( fabs(UnitVector.X()) <.3){
    if (print){
      cout << "\t\t\t"; cout << fabs(UnitVector.X()) << " is too slow\n";
    }
    Flag = 1;
    return;
  }
 switch(Wall){
  case 1: normal_vector = sign(UnitVector.X())*x_normal; break;
  case 2: normal_vector = sign(UnitVector.Y())*y_normal; break;
  case 3: normal_vector = sign(UnitVector.Z())*z_normal; break;
  default: cout << "please pick wall 1, 2, or 3\n"; return;
 }

  dotproduct = UnitVector.Dot(normal_vector);
  magnitudeproduct = UnitVector.Mag()*normal_vector.Mag();
  angle = acos (dotproduct / magnitudeproduct);

  if (print){
    cout << "\t\t"; cout << "dotproduct = " << dotproduct << endl;
    cout << "\t\t"; cout << "Photon Angle = " << angle << endl;
    cout << "\t\t"; cout << "Critical Angle = " << CriticalAngle << ", " << pi - CriticalAngle << endl;
  }
  if ((angle < CriticalAngle) || (angle > (pi - CriticalAngle)&&(angle < pi))){
    if (print){
      cout << "\t\t\t"; cout << "CRITICAL ANGLE ERROR!!\n";
    }
    Flag = 1;
  }
  if ((print) && (Flag)){
    cout << "\t\t\t"; cout << "Flag!!\n";
  }
}