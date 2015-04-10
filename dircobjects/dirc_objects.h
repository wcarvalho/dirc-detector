#ifndef __DIRC_OBJECTS__
#define __DIRC_OBJECTS__

#include <vector>
#include <cmath>
#include <string>
#include "TObject.h"
#include "TVector3.h"
#include "TH2D.h"
#include "particle_objects.h"
#include "photon_objects.h"
#include "analysis_objects.h"


//=============================================================================================
class Detector : public TObject
{
public:
  Detector(double l = 490., double w = 3.5, double h = 1.7) : Length(l), Width(w), Height(h),
  				Radial_D(100.), Mag_field(1.5), n(1.474), smear(.01),
  				x_normal(-1,0,0), y_normal(0,-1,0), z_normal(0,0,-1) {}
  ~Detector(){}

  void get_Critical_Angle(int n_out = 1){ CriticalAngle = asin(n_out/n); }
	void CheckAngle(double theta, double phi, int Wall, int &Flag, bool print);

  double Length;
  double Width;
  double Height;
	double Radial_D;									// distance from beam
	double Mag_field;
  double n;
  double CriticalAngle;
  double Resolution;
  double smear;
  TVector3 x_normal;
  TVector3 y_normal;
  TVector3 z_normal;


	int Checker;

  ClassDef(Detector, 1);

};


//=========================================================================================================================
//=========================================================================================================================


class GeneratorOut : public TObject
{
public:
	GeneratorOut(){}
	~GeneratorOut(){}

	std::vector<PhotonOut> Photons;
	std::vector<ParticleOut> Particles;

  ClassDef(GeneratorOut, 1);
};

class ReconstructionData : public TObject
{
public:
  ReconstructionData(){}
  ~ReconstructionData(){}

  int Particle;
  int EmissionAngle;

  ClassDef(ReconstructionData, 1);
};

class Reconstruction : public TObject
{
public:
  Reconstruction() {}
  ~Reconstruction(){}

  std::vector<ReconstructionData> Track;
	std::vector<std::vector<PhotonOut> > Photons;

  ClassDef(Reconstruction, 1);
};


#endif
