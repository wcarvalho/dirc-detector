#include <iostream>
#include <vector>
#include <string>
#include "dirc_objects.h"
#include "../headers/generator.h"
using namespace std;

/*================================================================================================
Push Back new Event (with parameters) into particle_events
================================================================================================*/

void PushBack_ParticleEvent(Detector detector, vector<ParticleEvent> &particle_events, int num_particles, int input, string Output)
{
	Output = "no";
  ParticleEvent particle_event;
  particle_events.push_back(particle_event);
  particle_events.back().Event = particle_events.size() - 1;

  Fill_ParticleEvent(detector, particle_events.back(), num_particles, input, Output);


}