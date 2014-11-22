#include "dirc_objects.h"

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
// #include <sys/time.h>

#include "TTree.h"
#include "TFile.h"
#include "TRandom3.h"
using namespace std;

void fillparameters(string const &settings, string &inputfile, string &outputfile, double &smear){

	ifstream f;
	f.open(settings.c_str());
	int size = 256;
	char val[size];
	int i = 0;
	while (f.good()){
		f.getline(val, size);
		if (i == 0) inputfile = val;
		if (i == 1)	outputfile = val;
		if (i == 2)	smear = atof(val);
		++i;
	}
}