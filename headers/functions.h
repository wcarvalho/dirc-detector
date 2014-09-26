#ifndef Unique1
#define Unique1

//------------------------------------------
//		C++
//------------------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <map>

//------------------------------------------
//		ROOT
//------------------------------------------
#include "TMath.h"
#include "TObject.h"
#include "TVector3.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TRandom3.h"



//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "random.h"
// #include "Rotater.h"

using namespace std;

// Class made for printing options
// ignore
class Displayer
{
public:
	Displayer() : Main("no"), Data("no"), Specific("no"), General("no"), location(0) {}
	~Displayer(){}

	void Angle(double theta, double phi);

	// void Vector_2D(vector< vector<double> > array);
	// void Array_1D(double *array, int w);
	// void Array_2D(double **array, int l, int w);

  string Main;
  string Action;
  string Important;
  string Data;
  string Specific;
  string General;
  string Trivial;
  int location;
  int checker;

};


// class used to rotate a unit vector into the frame given by a provided theta and phi
class Rotater
{
public:
	Rotater() : Zero(0,0,0), X(1,0,0), Y(0,1,0), Z(0,0,1), Direction("Outof") {}
	~Rotater(){}

	void Rotate_Photon(double &pho_theta, double &pho_phi);
	void Feed_Particle(double par_theta, double par_phi);

	string Direction;
	TVector3 RotationVector;
	TVector3 Zero;
  TVector3 X;
  TVector3 Y;
  TVector3 Z;
	double Angle;
	double Matrix[3][3];

private:
	void Set_Matrix();
};

// class which provides mass information based on provided Particle eta and pt
class mass
{
public:
	mass(double e = .2, double p = 3.): eta(e), pt(p)
		{ MassNameMapper(); AngleMassMapper(); }
	~mass(){}

	void MassNameMapper();
	void AngleMassMapper();

	std::map<double, std::string> MasstoName;
	std::map<double, double> AngletoMass;
	std::map<std::string, double> NametoMass;

	double eta;
	double pt;

};

//		Printing / Cosmetic
//------------------------------------------
void TabToLevel(int level);
void Print_TVector(TVector3 v);
void Print_2D_Vector(vector< vector<double> > array);
void Introduce(string title, string Output);
void Check_PhotonEvent(PhotonEvent event);
void Check_ParticleEvent(ParticleEvent event);



//		Data
//------------------------------------------
void Get_Photon_UnitVector(Photon &photon);
TVector3 Get_UnitVector(double theta, double phi);
// void Rotate_Photons_IntoFrame(Detector d, ParticleOut particle, vector<PhotonOut> &photons, int beg, int end, string Output);
void create_2D_vector(vector< vector<double> > &array, int rows, int number_of_columns);
void Update_Photon_ThetaPhi(Photon &photon, string Output);
void GraphFocus(TCanvas *c1, TGraph*& focus, PhotonEvent photon_event, string Output);
void GraphRest(TCanvas *c1, TGraph*& rest, PhotonEvent photon_event, string Output);
void GraphBounds(TCanvas *c1, string Output);

//		Return Values
//------------------------------------------
// double RandomDoubleBetween(double beginning, double end, int input_number);
// double RandomIntBetween(int beginning, int end, int input_number);
// double* random_numbers(int size, int input_number);
int Corresponding_Photons(PhotonEvent photon_event, int focus);
int Photons_Rest(PhotonEvent photon_event, int focus);
double *ExtractTheta(vector<Photon> v, string Output);
double *ExtractPhi(vector<Photon> v, string Output);



// void RandomGaus(double &value, double sigma, string Output);
string fixedLength(int value, int digits = 3);


#endif