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
	Simulate(double theta, double phi) : Vec(0.,0.,0.) { Theta=theta; Phi=phi; SetVec(Theta, Phi); }
	~Simulate(){}

	void SetDim(double l, double w, double h){ Dim[0] = l; Dim[1] = w; Dim[2] = h; }
	void SetStart(double x, double y, double z){ coord[0] = x; coord[1]=y; coord[2]=z; }
	void SetVec(double theta, double phi)
		{ TVector3 v(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)); Vec = v; GetTheta(); GetPhi(); }
	void GetTheta(){Theta = Vec.Theta(); }
	void GetPhi(){Phi = Vec.Phi(); }
	void FlipX(){ Vec.SetX(-Vec.X()); GetTheta(); GetPhi(); } 	
	void FlipY(){ Vec.SetY(-Vec.Y()); GetTheta(); GetPhi(); }
	void FlipZ(){ Vec.SetZ(-Vec.Z()); GetTheta(); GetPhi(); }
	void DistancetoWalls(string Output = "no");
	void WhichWall(string Output = "no");
	double WillTravel(){ return sqrt(
	    Vec(1)*TimeToWall*Vec(1)*TimeToWall+
	    Vec(2)*TimeToWall*Vec(2)*TimeToWall+
	    Vec(0)*TimeToWall*Vec(0)*TimeToWall
	                           ); }
	void TravelDistance(double d);

	void GotoWall(string Output);

	double Distance[3];
	double Dim[3];
	double TimesToWall[3];
	double TimeToWall;
	int wall;
	TVector3 Vec;
	int Flag;

};

#endif