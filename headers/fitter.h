//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream> 

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TTree.h"
#include "TFile.h"
#include "TF1.h"
#include "TH2D.h"
#include "TSpectrum.h"

//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
#include "Simulate.h"
//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &projectedlost);