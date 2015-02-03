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

class Identifier : public TObject
{
public:
	Identifier(){}
	~Identifier(){}

	double FitParticle1D(TCanvas* c, TH1D &h, vector< double > &params, double xlow, double xhi, double center, double width, string name, bool print);
	void reverseprobabilitymap();

	std::map<double, std::vector<TObject> > particle1Dfits;
	std::map<double, std::vector<TObject> > particle2Dfits;
	std::map<string, double> probabilitymap;
	std::map<double, string> rprobabilitymap; 	// reversed for finding key associated with value
};

class LookUpTable
{
public:
	LookUpTable(){}
	LookUpTable(int ndim, double len_low[], double len_hi[], int nbin[],
	            string file, int whichCase) : Case(whichCase), print(false) {
		setNDimension(ndim);
		setLengths(len_low, len_hi);
		setNBins(nbin);
		tableFromTxt(file);
	}
	~LookUpTable(){}

	void setNDimension(int n);
	void setLengths(double givenLengths_low[], double givenLengths_hi[]);
	void setNBins(int givenBins[]);
	void tableFromTxt(string file);
	void setInterpolationMethod(int givenCase){ Case = givenCase; }

	std::pair<double, double> getMeanRmsPair(double values[]);
	double mean(double values[]){ return getMeanRmsPair(values).first; }
	double rms(double values[]){ return getMeanRmsPair(values).second; }

	void getBin(double values[], int bin[]);
	void getBinCenters(int bin[], double centers[]);
	int getGlobalBin(int bin[]);
	double getMean(int globalBin);
	double getRMS(int globalBin);

	void Interpolate(double values[], double &mean, double &rms);
	void NoInterpolation(double values[], double &mean, double &rms);
	void LinearInterpolation(double values[], double &mean, double &rms);
	void SetPrint(bool Set = true){ print = Set; }
	void close();

	int N;								// number of dimensions
	int nValues;					// total number of values in lookuptable
	int Case;

	std::vector < int > nbins;
	std::vector < double > mins;
	std::vector < double > lengths;
	std::vector < double > steps;

	std::vector < double > MEAN;
	std::vector < double > RMS;

	bool print;


};

double LookUpTableWrapper(double const& x, double const& y, double const& theta, double const& phi, double const& v);

double RiemannSum(double const& x, double const& y, double const& theta, double const& phi, double const& v);

void FindLostPhotons(double x, double y, double theta, double phi, double eta, double pt, double l, double w, double h, map<double, double> &madeit, double &traveled, bool print);
