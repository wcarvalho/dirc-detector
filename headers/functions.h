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






//==========================================
//		Functions
//==========================================
using namespace std;

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
void Get_Particle_UnitVector(Particle &particle);
void Rotate_Photons(Detector d, ParticleEvent particle_event, PhotonEvent &photon_event, string Output);
void Rotate_Photons_IntoFrame(Detector d, ParticleOut particle, vector<PhotonOut> &photons, int beg, int end, string Output);
void create_2D_vector(vector< vector<double> > &array, int rows, int number_of_columns);
void Fill_RotationMatrix(vector< vector<double> > &matrix, TVector3 Tvector, double angle, string Output);
void Update_Photon_ThetaPhi(Photon &photon, string Output);
void GraphFocus(TCanvas *c1, TGraph*& focus, PhotonEvent photon_event, string Output);
void GraphRest(TCanvas *c1, TGraph*& rest, PhotonEvent photon_event, string Output);
void GraphBounds(TCanvas *c1, string Output);
// void Print_Histogram(string dir, string name, double theta, double phi, double x, double y, TH2D h);

//		Return Values
//------------------------------------------
double RandomNumberBetween(double beginning, double end, int input_number);
double* random_numbers(int size, int input_number);
int Corresponding_Photons(PhotonEvent photon_event, int focus);
int Photons_Rest(PhotonEvent photon_event, int focus);
TVector3 Rotation_Vector(Detector d, Particle particle, string Output);
TVector3 Rotated_Vector(vector< vector<double> > R, TVector3 vector, string Output);
double *ExtractTheta(vector<Photon> v, string Output);
double *ExtractPhi(vector<Photon> v, string Output);


// SimulateEvent();
// void Scan_X(double x_low, double x_high, vector< vector<Particle> > &particle_events, vector< vector<Photon> > &photon_events, Dector &detector);