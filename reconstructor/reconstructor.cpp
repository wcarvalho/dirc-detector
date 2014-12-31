#include "dirc_objects.h"

void removeFirstParticles(bool Remove, GeneratorOut*& event_output, int last){
	if (Remove){
      int beginning = event_output->Particles.size() - last;
      if (beginning>0){
	      for (unsigned int j = 0; j < beginning; ++j)
	      	event_output->Particles.erase(event_output->Particles.begin());
      }
    }
}