#include <iostream>
#include <string>
#include <math.h>
#include "TVector3.h"
#include "TMath.h"
#include "dirc_objects.h"
#include "../headers/simulator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectories for a single Photon of a single "Event"
================================================================================================*/

void CheckAngel(Detector d, Photon &photon, string Output)
{
	if (Output == "yes")
	{
		TabToLevel(4); cout << "CheckAngel:\n";
	}
	double pi = TMath::Pi();
	double angle;
	double dotproduct, magnitudeproduct;
  TVector3 normal_vector;
  TVector3 x_normal(-1,0,0);
  TVector3 y_normal(0,-1,0);
  TVector3 z_normal(0,0,-1);
  if(photon.Wall == 1)
  {
  	normal_vector = x_normal;
  	Check_PhotonDirection(photon.UnitVector.X(), normal_vector);
  }

  if(photon.Wall == 2)
  {
  	normal_vector = y_normal;
  	Check_PhotonDirection(photon.UnitVector.Y(), normal_vector);
  }

  if(photon.Wall == 3)
  {
  	normal_vector = z_normal;
  	Check_PhotonDirection(photon.UnitVector.Z(), normal_vector);
  }


  dotproduct = photon.UnitVector.Dot(normal_vector);
  magnitudeproduct = photon.UnitVector.Mag()*normal_vector.Mag();
  angle = acos (dotproduct / magnitudeproduct);
  // if (Output == "yes")
  // {
		// TabToLevel(5); cout << "dotproduct = " << dotproduct << endl;
		// TabToLevel(5); cout << "magnitudeproduct = " << magnitudeproduct << endl;
		// TabToLevel(5); cout << "Photon Angle = " << angle << endl;
		// TabToLevel(5); cout << "Critical Angle = " << d.CriticalAngle << ", " << pi - d.CriticalAngle << endl;
  // }
 //  if (photon.Flag == 1)
	// {
	// 	cout << "photon " << photon.Which << endl;
	// 	cout << "reflections = " << photon.Reflections << endl;
	// }
  if ((angle < d.CriticalAngle) || (angle > (pi - d.CriticalAngle)&&(angle < pi)))
  {
    // if (Output == "yes")
    // {
      // TabToLevel(3); cout << "CRITICAL ANGLE ERROR!!\n";
    // }
    photon.Flag = 1;
  }
  if ( fabs(photon.UnitVector.X()) <.3)
  {
    // TabToLevel(3); cout << "TOO SLOW!!\n";
    photon.Flag = 1;
  }
  if ((Output == "yes")&&(photon.Flag == 1)){ TabToLevel(3); cout << "Flag!!\n"; }
}