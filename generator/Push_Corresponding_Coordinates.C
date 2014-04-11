#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "../headers/generator.h"
#include "../headers/functions.h"

using namespace std;

/*================================================================================================
Simulate the trajectory for a single Particle of a single "Event"
================================================================================================*/

void Push_Corresponding_Coordinates(Particle particle, PhotonEvent &photon_event, string Output)
{
	int emission = 0;
	int photon_number = 0;
	int emission_photon = 0;
	int photons_per_emission = particle.NumberofPhotons/particle.Emissions;
	double Default_X = particle.X;
	double Default_Y = particle.Y;
	double Default_Z = particle.Z;
	photon_event.iterator = 0;	
	// cout << particle.NumberofPhotons << endl; exit(1);
	if (Output == "yes")
	{
		TabToLevel(4); cout << "Push_Corresponding_Coordinates:\n";
	}

	if (Output == "yes")
	{ 
		TabToLevel(5); cout << "Incident_X = " << Default_X << endl;
		TabToLevel(5); cout << "Incident_Y = " << Default_Y << endl;
		TabToLevel(5); cout << "Incident_Z = " << Default_Z << endl;
	}

	for (emission = 0; emission < particle.Emissions; emission++)
	{
		if (Output == "yes") { TabToLevel(4); cout << "Set "<< emission << ":\n";}
		particle.X = Default_X;
		particle.Y = Default_Y;
		particle.Z = Default_Z;
		// Get_ParticleCoordinate(EmissionTimes[emission], particle, Output);
		emission_photon = 0;
		while(emission_photon < photons_per_emission)
		{
			// if (Output == "yes") { TabToLevel(5); cout << "Photon " << photon_number << endl; }
			if(photon_event.Photons[photon_number].WhichParticle == particle.Which)
			{
				photon_event.Photons[photon_number].X = particle.X;
				photon_event.Photons[photon_number].Y	= particle.Y;
				photon_event.Photons[photon_number].Z = particle.Z;
				emission_photon++;
				photon_event.iterator++;
				if (Output == "yes")
				{
					TabToLevel(5); cout << "Photon " << photon_number << endl;
					TabToLevel(6); cout << "Photon X = " << photon_event.Photons[photon_number].X << endl;
					TabToLevel(6); cout << "Photon Y = " << photon_event.Photons[photon_number].Y << endl;
					TabToLevel(6); cout << "Photon Z = " << photon_event.Photons[photon_number].Z << endl;
				}
			}
			photon_number++;
		}
	}

	while (photon_event.iterator < particle.NumberofPhotons)
	{
		// if (Output == "yes") { TabToLevel(5); cout << "photon " << photon_number << endl; }
		if(photon_event.Photons[photon_number].WhichParticle == particle.Which)
		{
			photon_event.Photons[photon_number].X = particle.X;
			photon_event.Photons[photon_number].Y	= particle.Y;
			photon_event.Photons[photon_number].Z = particle.Z;
			photon_event.iterator++;
			if (Output == "yes")
			{
				TabToLevel(5); cout << "photon " << photon_number << endl;
				TabToLevel(6); cout << "Photon X = " << photon_event.Photons[photon_number].X << endl;
				TabToLevel(6); cout << "Photon Y = " << photon_event.Photons[photon_number].Y << endl;
				TabToLevel(6); cout << "Photon Z = " << photon_event.Photons[photon_number].Z << endl;
			}
		}
		photon_number++;
	}
}