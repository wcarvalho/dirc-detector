#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
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
#include "TTree.h"
#include "TFile.h"
#include "TGraph.h"

//------------------------------------------
//		mine
//------------------------------------------
#include "dirc_objects.h"
#include "functions.h"
#include "Parameters.h"
#include "Simulate.h"
#include "file-properties.h"

//------------------------------------------
//		Functions
//------------------------------------------
using namespace std;


void Release_Photons(Simulate simPar, PhotonEvent &event, int released, double EmissionAngle);

void find_critical_angle(double &critical_angle, string Output);
void PushBack_ParticleEvent(Detector detector, vector<ParticleEvent> &particle_events, Parameter param, int input, string Output);
void Fill_ParticleEvent(Detector d, ParticleEvent &event, ParticleParameters parameters, int num_particles, int input, string Output);
void PushBack_PhotonEvent(ParticleEvent &particle_event, vector<PhotonEvent> &photon_events, int input, string Output);
void SimulateEvent(Detector d, ParticleEvent &particle_event, PhotonEvent &photon_event, Displayer Output);
void Simulate_PhotonPath(Detector d, Photon &photon, double smear = .01, bool print = false);
void Simulate_ParticlePath(Detector d, Particle &particle, int parnum, PhotonEvent &photon_event, double avg_d, bool print);
void CheckForFlag(PhotonEvent &photon_event, int &iterator, string Output);
void Get_ParticleDistance(Detector d, Particle &particle, string Output);
void Find_Particle_X_Distance(Detector d, Particle &particle, string Output);
void Find_Particle_Y_Distance(Detector d, Particle &particle, string Output);
void Find_Particle_Z_Distance(Detector d, Particle &particle, string Output);
void Get_ParticleWall(Particle &particle, string Output);
double *Random_Emission_Times(Particle particle, string type, string Output);
void Push_Corresponding_Coordinates(Particle particle, PhotonEvent &photon_event, string Output);
void Get_ParticleCoordinate(double t, Particle &particle, string Output);
void Get_PhotonDistance(Detector d, Photon &photon, string Output);
void Find_Photon_X_Distance(Detector d, Photon &photon, string Output);
void Find_Photon_Y_Distance(Detector d, Photon &photon, string Output);
void Find_Photon_Z_Distance(Detector d, Photon &photon, string Output);
void Get_PhotonWall(Photon &photon, string Output);
void CheckAngel(Detector d, Photon &photon, string Output);
void Check_PhotonDirection(double v, TVector3 &normal_vector);
void Move_Photon(Photon &photon, string Output);
void Change_Angle(Photon &photon, string Output);
void GenerateEvent(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
                   Parameter param, TTree* tree, int input, Displayer Output);
void FillTree(TTree &tree, ParticleEvent particle_event, PhotonEvent photon_event, GeneratorOut& event_output, string Output, GeneratorOut* event_outputCopy = 0, bool Append = false, bool quiet = false);
void ScanTheta(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_theta, double ending_theta, double theta_step,
							TTree* tree, int input, Displayer Output);
void ScanX(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_x, double ending_x, double x_step,
							TTree* tree, int input, Displayer Output);
void ScanY(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_y, double ending_y, double y_step,
							TTree* tree, int input, Displayer Output);
void ScanPhi(Detector detector, vector<ParticleEvent> &particle_events, vector<PhotonEvent> &photon_events,
              Parameter param, GeneratorOut& event_output,
              double beginning_phi, double ending_phi, double phi_step,
							TTree* tree, int input, Displayer Output);


void Pull_Particles(ParticleEvent particle_event, GeneratorOut& event_output, string Output);
void Pull_Photons(PhotonEvent photon_event, GeneratorOut& event_output, string Output);

// void Scan_X(double x_low, double x_high, vector< vector<Particle> > &particle_events, vector< vector<Photon> > &photon_events, Dector &detector);