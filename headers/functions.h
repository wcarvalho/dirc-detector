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
#include "myrandom.h"
// #include "Rotater.h"

using namespace std;


//		Printing / Cosmetic
//------------------------------------------
void TabToLevel(int level);
void Print_TVector(TVector3 v);
void Print_2D_Vector(vector< vector<double> > array);
void Introduce(string title, string Output);
void Check_PhotonEvent(PhotonEvent event);
void Check_ParticleEvent(ParticleEvent event);
void addtoFile(std::string file, std::string toAdd);



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
double getMin(std::map<std::string, double> mymap);


// void RandomGaus(double &value, double sigma, string Output);
string fixedLength(int value, int digits = 3);
string returnEnd(std::string of, std::string from);
string returnBeg(std::string of, std::string from);
string copyFile(std::string file, std::string suffix, std::string ending = "1");
std::string getFilename(std::string path);
#endif