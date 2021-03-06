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
#include "TMultiGraph.h"
#include "dirc_objects.h"
using namespace std;

class TrackRecon : public TObject
{
public:
	TrackRecon(){}
	~TrackRecon(){}

	void clear();
	void printLatest();
	void LinkHistogram(TH1D* h){ h = &Final1D; }
	void addFitsToHistogram(TH1D &h);
	void addFitToHistogram(TH1D &h, std::string type);
	int size() const { return Options.size(); }
	int size(){ return Options.size(); }
	int NReconstructions(){ return Options.size(); } // deprecated
	int getIndexOf(std::string type) const;
	double getLatestIntegralCenter(){ return Params.back().at(1); }
	double getLatestIntegralCenter() const { return Params.back().at(1); }
	double getIntegralCenterAt(int const i){ return Params.at(i).at(1); }
	double getIntegralCenterAt(int const i) const { return Params.at(i).at(1); }
	bool passed_intensity_cut (int const i, bool print = false, double const threshold = 10) const;
	double getSigmaThetaAt(int const i) const { return SigTheta.at(i); }
	double getSigmaAreaAt(int const i) const { return SigArea.at(i); }
	double getnSigmaAreaAt(int const i, bool print = false) const {
		if (print) cout << "\t" << getNameAt(i) << " : \u0394\u03C3 (intensity) = " << delSigArea.at(i) << endl;
		return delSigArea.at(i);
	}
	double getnSigmaThetaAt(int const i, bool print = false) const {
		if (print) cout << "\t" << getNameAt(i) << " : \u0394\u03C3 (theta) = " << delSigTheta.at(i) << endl;
		return delSigTheta.at(i);
	}
	double getnSigmaOf(std::string type, bool const print = false) const;
	double getnSigmaAt(int const i, bool const print = false) const;



	double getIntegralAt(int const i) const { return Areas.at(i); }
	double getIntegralHeightAt(int const i) const { return Params.at(i).at(0); }
	double getIntegralConstantAt(int const i) const { return Params.at(i).at(3); }
	std::string getBestFit(double const threshold = 10, bool print = false) const;
	std::string getNameAt(int const i) const { return Options.at(i); }
	double getExpectedIntensityAt(int const i) const {return ExpectedNumber.at(i); }
	double getExpectedAngleAt(int const i) const {return ExpectedAngle.at(i); }


	TH2D Hist2D;
	TH1D Final1D;

	std::vector < std::string > Options; 			// every particle type
	std::vector < double > SigTheta;				// delta sigma theta for every particle
	std::vector < double > delSigTheta;				// delta sigma theta for every particle
	std::vector < double > SigArea;				// delta sigma area for every particle
	std::vector < double > delSigArea;				// delta sigma area for every particle
	std::vector < double > Sigmas;						// delta sigma for every particle (quadrature sum)
	std::vector < double > Areas;							// calculated area under particle fit for every particle
	std::vector < double > ExpectedNumber;		// expected number of photons for every particle
	std::vector < double > ExpectedAngle;		// expected angle for every particle
	std::vector < std::vector < double> > Params;	// one gaussian fit per particle. each fit contains height, center, width, constant, xlow, xhi
	std::vector < std::string > Funs;					// function used for each function

	ClassDef(TrackRecon, 1);
};

class TrackRecons : public TObject
{
public:
	TrackRecons() {}
	~TrackRecons(){}

	void AddTrackRecon();
	void PushBackParams();
	// void CreateIndexedPlot(TCanvas &C);
	// void SetIndex(std::vector< int > i){ index = std::move(index) ; }
	// void SetPhotonPlot(int const i, TCanvas C){
	// 	ColoredPhotons.at(i) = *C.Clone();
	// }

	std::vector< TrackRecon > Recon;
	std::vector < int > index;
	// std::vector < TCanvas > ColoredPhotons;			// One Coloring set per particle_type

	ClassDef(TrackRecons, 1);
};







///////////////// Deprecated  //////////////
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

	std::vector<TH1D> Hists1D;
	std::vector<TH2D> Hists2D;
	std::vector< std::vector<double> > data;
	std::vector< double > data1D;
	bool print;

	ClassDef(Analysis, 1);
};

#endif


int getReconIndex(TrackRecon &T, std::string desired_type);
