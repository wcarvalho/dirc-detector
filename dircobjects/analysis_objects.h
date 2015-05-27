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
	int getIndexOf(std::string type);
	double getLatestIntegralCenter(){ return Params.back().at(1); }
	double getLatestIntegralCenter() const { return Params.back().at(1); }
	double getIntegralCenterAt(int const i){ return Params.at(i).at(1); }
	double getIntegralCenterAt(int const i) const { return Params.at(i).at(1); }
	double getnSigmaAreaAt(int const i) const {
		if (getIntegralAt(i) < 10) return 1.e100;
		else return delSigArea.at(i);
	}
	double getnSigmaThetaAt(int const i) const {
		if (getIntegralAt(i) < 10) return 1.e100;
		else return delSigTheta.at(i);
	}
	double getIntegralAt(int const i) const { return Areas.at(i); }
	double getIntegralHeightAt(int const i) const { return Params.at(i).at(0); }
	std::string getBestFit(double const threshold);
	std::string getNameAt(int const i) const { return Options.at(i); }


	TH2D Hist2D;
	TH1D Final1D;

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
	TrackRecons() {}
	~TrackRecons(){}

	void AddTrackRecon();
	void PushBackParams();
	void CreateIndexedPlot(TCanvas &C);
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
