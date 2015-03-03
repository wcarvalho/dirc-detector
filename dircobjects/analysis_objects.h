#ifndef __ANALYSIS_OBJECTS__
#define __ANALYSIS_OBJECTS__

#include "iostream"
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include "TObject.h"
#include "TVector3.h"
#include "TH2D.h"
#include "TF1.h"
#include "TCanvas.h"
#include "dirc_objects.h"

class TrackRecon : public TObject
{
public:
	TrackRecon(){}
	~TrackRecon(){}

	void clear();
	void printLatest();

	TH1D Hist;
	TH2D Hist2D;
	std::vector < std::string > Options; 			// every particle type
	std::vector < double > delSigTheta;				// delta sigma theta for every particle
	std::vector < double > delSigArea;				// delta sigma area for every particle
	std::vector < double > Sigmas;						// delta sigma for every particle (quadrature sum)
	std::vector < double > Areas;							// calculated area under particle fit for every particle
	std::vector < double > ExpectedNumber;		// expected number of photons for every particle
	std::vector < std::vector < double> > Params;	// one gaussian fit per particle. each fit contains height, center, width, constant, xlow, xhi
	std::vector < std::string > Funs;					// function used for each function

	ClassDef(TrackRecon, 1);
};

class TrackRecons : public TObject
{
public:
	TrackRecons(){}
	~TrackRecons(){}

	void AddTrackRecon();
	void PushBackParams();
	void FitGaussianPlusConstant(TH1D &h, double xlow, double xhi, double &center, double centerbounds[2], double &width, double widthbounds[2], double &Area);

	std::vector<TrackRecon> Recon;

	ClassDef(TrackRecons, 1);
};
// makes and stores histograms for an event
class Analysis : public TrackRecons
{
public:
	Analysis(bool p = false) : print(p) {}
	~Analysis(){}

	void SetData(std::vector< std::vector<double> > data2d){
		data = data2d;
	}
	void AddTH1D(const char* name, const char* title, int nbinsx, double xlow, double xup, int which);
	void AddTH2D(const char* name, const char* title, int nbinsx, double xlow, double xup, int nbinsy, double ylow, double yup);

	void SetPrint(bool p = true) {print = p;}
	void FitGaussianPlusConstant(double xlow, double xhi, double& center, double centerbounds[2], double& width, double widthbounds[2], double &Area);

	std::vector<TH1D> Hists1D;
	std::vector<TH2D> Hists2D;
	std::vector< std::vector<double> > data;
	std::vector< double > data1D;
	bool print;

	ClassDef(Analysis, 1);
};

#endif
