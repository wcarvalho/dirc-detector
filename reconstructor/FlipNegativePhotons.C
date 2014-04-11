#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "TCanvas.h"
#include "../headers/reconstructor.h"
#include "../headers/functions.h"


using namespace std;

/*================================================================================================
Change all photons so that they have hopefully* had an "even" number of reflections
================================================================================================*/
void FlipNegativePhotons(GeneratorOut*& event_output, string Output)
{

	int i;

	if (Output == "yes")
	{
		TabToLevel(2); cout << "FlipNegativePhotons:\n";
	}

	for (i = 0; i < event_output->Photons.size(); i++)
	{
		FlipNegativePhoton(event_output->Photons.at(i), Output);
	}

}
