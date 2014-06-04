#include "dirc_objects.h"
#include "../headers/functions.h"
#include "../headers/random.h"
#include "../headers/generator.h"

//						generate_Event Class
//=============================================================================================
// generates an event with npar random particles
// void generate_Event::generate(int npar, int &input)
// {
// 	for (unsigned int par = 0; par < npar; par++)
// 	{
// 		NewParticle();
// 		gParticle sp(input); if (input != 0){ input++; }
// 	}
// }

// // void generate_Event::push_par()
// // copies members to parent class members
// void generate_Event::extract_sEvent(sEvent &ev)
// {
// 	ev.sPar = sPar;
// 	ev.Par = Par;
// }