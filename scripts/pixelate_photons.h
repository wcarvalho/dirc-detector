#include "dirc_objects.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// #include <sys/time.h>

#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"

int nbins(double bounds[2], double resolution){
	return int( (bounds[1]-bounds[0]) / resolution);
}