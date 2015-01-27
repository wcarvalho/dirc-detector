#include <iostream>
#include <vector>
#include <string>
#include "TMath.h"
#include "dirc_objects.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Check Values of a Particle Event
================================================================================================*/

void Check_ParticleEvent(ParticleEvent event)
{
	int i;
	TabToLevel(2); cout << "ParticleEvent " << event.Event << " wih focus " << event.Focus << ":\n";
	TabToLevel(3); printf
	("Which \t|Photons \t|Phi \t\t|Theta \t\t|X \t\t|Y \t\t|Z \t\t|\n");
	for(i = 0; i < event.Particles.size(); i++)
	{
  	TabToLevel(3); printf
  	("%i \t|%i \t\t|%f \t|%f \t|%f \t|%f \t|%f \t|\n",
  	event.Particles[i].Which, event.Particles[i].NumberofPhotons,
  	event.Particles[i].Phi, event.Particles[i].Theta,
  	event.Particles[i].X, event.Particles[i].Y, event.Particles[i].Z);
	}
}