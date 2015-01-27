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

		  for (k = 0; k < photon_event.Photons.size(); k++)
		  {
		  	if (photon_event.Photons[k].WhichParticle == i)
		  	{
			  	if (Output == "yes") {TabToLevel(3); cout << "Photon " << k << " with Theta " << particle_event.Particles[i].Theta << "\n"; }
			  	photon_event.Photons[k].UnitVector = Rotated_Vector(RotationMatrix, photon_event.Photons[k].UnitVector, Output);
			  	Update_Photon_ThetaPhi(photon_event.Photons[k], Output);
		  	}
		  }	
		}

	}



}