//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream> 
#include <sys/time.h>

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
#include "file-properties.h"
//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;

class Identifier : public TObject 
{
public:
	Identifier(){}
	~Identifier(){}

	double FitParticle1D(TCanvas* c, TH1D &h, vector< double > &params, double xlow, double xhi, double center, double width, string name, string prefix, bool make, bool print);
	void reverseprobabilitymap();

	std::map<double, std::vector<TObject> > particle1Dfits;
	std::map<double, std::vector<TObject> > particle2Dfits;
	std::map<string, double> probabilitymap;
	std::map<double, string> rprobabilitymap; 	// reversed for finding key associated with value
};

void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &madeit, double &traveled, bool print);


