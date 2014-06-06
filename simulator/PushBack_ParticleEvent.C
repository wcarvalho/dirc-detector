#include <iostream>
#include <vector>
#include <string>
#include "dirc_objects.h"
#include "../headers/simulator.h"
using namespace std;

/*================================================================================================
Push Back new Event (with parameters) into particle_events
================================================================================================*/

void PushBack_ParticleEvent(Detector detector, vector<ParticleEvent> &particle_events, Parameter param, int input, string Output)
{
  ParticleEvent particle_event;
  Random r;
  int num_particles;
  particle_events.push_back(particle_event);
  particle_events.back().Event = particle_events.size() - 1;

  r.Int(param.Particles[0], param.Particles[1], num_particles);

  Fill_ParticleEvent(detector, particle_events.back(), param.Particle, num_particles, input, Output);


}