#include <iostream>
#include <vector>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "TVector3.h"


using namespace std;

/*================================================================================================
Rotate Photons in event to the frame of either the detecotr if in phase generation or of the particle if in frame reconstruction
================================================================================================*/


void Rotate_Photons_IntoFrame(Detector d, ParticleOut particle, vector<PhotonOut> &photons, int beg, int end, string Output)
{

	int k;
	double x,y,z;
	double rotation_angle;
	x = sin(particle.Theta)*cos(particle.Phi);
	y = sin(particle.Theta)*sin(particle.Phi);
	z = cos(particle.Theta);
	TVector3 Particle_v(x, y, z);
	TVector3 RotationVector;
	TVector3 ZeroVector(0,0,0);
	TVector3 Photon_v;
	double RotationMatrix[3][3];

	Rotater r;
	
	RotationVector = d.Z_UnitVector.Cross(Particle_v);
	if (RotationVector == ZeroVector) { TabToLevel(3); cout << "ZERO VECTOR\n"; return; }

	rotation_angle = d.Z_UnitVector.Angle(Particle_v);
	rotation_angle = -rotation_angle;

	if (Output == "yes") { TabToLevel(3); cout << "rotation_angle = " << rotation_angle << endl; }

  Output ="";
  r.RotationMatrix(RotationMatrix, RotationVector, rotation_angle, Output);

  for (k = beg; k < end; k++)
  {
		x = sin(photons.at(k).Theta)*cos(photons.at(k).Phi);
		y = sin(photons.at(k).Theta)*sin(photons.at(k).Phi);
		z = cos(photons.at(k).Theta);
		Photon_v.SetX(x);
		Photon_v.SetY(y);
		Photon_v.SetZ(z);
		// cout << "Theta = " << Photon_v.Theta() << endl;
		// TabToLevel(3); cout << "Unit Vector: ";Print_TVector(Photon_v);
		Photon_v = r.Rotated_Vector(RotationMatrix, Photon_v, Output);
		// TabToLevel(4); cout << "->";Print_TVector(Photon_v);
		// cout << "Theta = " << Photon_v.Theta() << endl;
  	photons.at(k).Theta = Photon_v.Theta();
  	photons.at(k).Phi = Photon_v.Phi();
  }	
}
