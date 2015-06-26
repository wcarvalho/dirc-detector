#ifndef simulate
#define simulate

//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TGraph.h"


//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"

using namespace std;

// This class can simulate the trajectory of a particle through a rectangular prism
//_____________________________________________________________________________________________
class Simulate: public sPhoton
{
public:
	Simulate(double theta, double phi) : Vec(0.,0.,0.), time_traveled(0.) { SetAngle(theta, phi); }
	~Simulate(){}

	void SetAngle(double t = 0., double p = 0.){
		Theta=t; Phi=p; SetVec(Theta, Phi);
	}
	void resetTime(){ time_traveled = 0.; }
	// void getTime(){ return time_traveled; }
	void setTime(double const t){ time_traveled = t; }
	void SetDim(double l, double w, double h){ Dim[0] = l; Dim[1] = w; Dim[2] = h; }
	void SetStart(double x, double y, double z){ coord[0] = x; coord[1]=y; coord[2]=z; }
	void SetVec(double theta, double phi)
		{ TVector3 v(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); Vec = v; GetTheta(); GetPhi(); }
	void SetVelocity(double v){ Vec.SetMag(v); }
	void GetTheta(){Theta = Vec.Theta(); }
	void GetPhi(){Phi = Vec.Phi(); }
	void FlipX(){ Vec.SetX(-Vec.X()); GetTheta(); GetPhi(); }
	void FlipY(){ Vec.SetY(-Vec.Y()); GetTheta(); GetPhi(); }
	void FlipZ(){ Vec.SetZ(-Vec.Z()); GetTheta(); GetPhi(); }
	void DistancetoWalls(bool print = false);
	void OldWhichWall(bool print = false);
	void WhichWall(bool print = false);
	double WillTravel();
	double TimeForDistance(double D, bool print = false);
	void TravelDistance(double d, bool print = false);
	void GotoWall(bool print = false);
	void Reflect(bool print = false);
	void PrintVec();
	void ResetTime(){ time_traveled = 0.; }
	void GoTo(int const whichcoordinate, double const value);
	double GetTimeTraveled(){
		return time_traveled; // returns units in s*1.e-9
	}

	double Distance[3];
	double Dim[3];
	double TimesToWall[3];
	double TimeToWall;
	int wall;
	TVector3 Vec;
	int Flag;
	double time_traveled;

};

#endif