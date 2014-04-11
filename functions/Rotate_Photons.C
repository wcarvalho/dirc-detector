#include <iostream>
#include <vector>
#include "dirc_objects.h"
#include "../headers/functions.h"
#include "TVector3.h"


using namespace std;

/*================================================================================================
Rotate Photons in event to the frame of either the detecotr if in phase generation or of the particle if in frame reconstruction
================================================================================================*/

void Rotate_Photons(Detector d, ParticleEvent particle_event, PhotonEvent &photon_event, string Output)
{

	int i, k;
	int Photons_Affected;
	TVector3 RotationVector;
	TVector3 ZeroVector(0,0,0);
	double rotation_angle;
	vector< vector<double> > RotationMatrix;

	Introduce("Rotate_Photons", Output);

	for(i = 0; i<particle_event.Particles.size(); i++)
	{
		RotationVector = Rotation_Vector(d, particle_event.Particles[i], Output);
		if (RotationVector == ZeroVector) { continue; }
		else
		{
			rotation_angle = d.Z_UnitVector.Angle(particle_event.Particles[i].UnitVector);
			if (photon_event.Phase == "Reconstruction")
			{
				rotation_angle = -rotation_angle;
			}
			if (Output == "yes") { TabToLevel(3); cout << "rotation_angle = " << rotation_angle << endl; }


		  create_2D_vector(RotationMatrix, 3, 3);
		  Fill_RotationMatrix(RotationMatrix, RotationVector, rotation_angle, Output);

		  Photons_Affected = Corresponding_Photons(photon_event, i);

		  for (k = 0; k < photon_event.Photons.size(); k++)
		  {
		  	if (Output == "yes") {TabToLevel(3); cout << "Photon " << k << " with Theta " << particle_event.Particles[i].Theta << "\n"; }
		  	if (photon_event.Photons.at(k).WhichParticle == i)
		  	{
			  	photon_event.Photons.at(k).UnitVector = Rotated_Vector(RotationMatrix, photon_event.Photons.at(k).UnitVector, Output);
			  	Update_Photon_ThetaPhi(photon_event.Photons.at(k), Output);
		  	}
		  }	
		}

	}
}





void Rotate_Photons_IntoFrame(Detector d, ParticleOut particle, vector<PhotonOut> &photons, int beg, int end, string Output)
{

	int k;
	double x,y,z;
	double rotation_angle;
	vector< vector<double> > RotationMatrix;
	x = sin(particle.Theta)*cos(particle.Phi);
	y = sin(particle.Theta)*sin(particle.Phi);
	z = cos(particle.Theta);
	TVector3 Particle_v(x, y, z);
	TVector3 RotationVector;
	TVector3 ZeroVector(0,0,0);
	TVector3 Photon_v;
	RotationVector = d.Z_UnitVector.Cross(Particle_v);
	if (RotationVector == ZeroVector) { TabToLevel(3); cout << "ZERO VECTOR\n"; return; }

	rotation_angle = d.Z_UnitVector.Angle(Particle_v);
	rotation_angle = -rotation_angle;

	if (Output == "yes") { TabToLevel(3); cout << "rotation_angle = " << rotation_angle << endl; }

  Output ="";
  create_2D_vector(RotationMatrix, 3, 3);
  Fill_RotationMatrix(RotationMatrix, RotationVector, rotation_angle, Output);

	TabToLevel(3); cout << "Example: Photon theta = " << photons.at(20).Theta << endl;
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
		Photon_v = Rotated_Vector(RotationMatrix, Photon_v, Output);
		// TabToLevel(4); cout << "->";Print_TVector(Photon_v);
		// cout << "Theta = " << Photon_v.Theta() << endl;
  	photons.at(k).Theta = Photon_v.Theta();
  	photons.at(k).Phi = Photon_v.Phi();
  }	
	TabToLevel(3); cout << "Photon theta = " << photons.at(20).Theta << endl;
}
