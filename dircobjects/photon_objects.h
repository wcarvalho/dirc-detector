#ifndef __PHOTON_OBJECTS__
#define __PHOTON_OBJECTS__

#include <vector>
#include <cmath>
#include <sstream>
#include <string>
#include "TObject.h"
#include "TVector3.h"
#include "TMath.h"
#include "TH2D.h"
#include "dirc_objects.h"

class PhotonOut : public TObject
{
public:
	PhotonOut(double theta=0., double phi=0.) : Theta(theta), Phi(phi), X(0.), Y(0.), Z(0.), Time_Traveled(0.) {}
  ~PhotonOut(){}

  virtual void SetAngle(double const t, double const p){ Theta = t; Phi = p; }
  friend std::ostream& operator<<(std::ostream& os, PhotonOut& p){
    os << p.Theta << ", " << p.Phi;
    return os;
  }
  std::string printcoord(){
    static std::stringstream ss; ss.str("");
    ss << X << ", " << Y << ", " << Z;
    return ss.str();  }
  void SetVelocity(double const v) { velocity = v; }
  double Theta;
  double Phi;
  double X;
  double Y;
  double Z;
  double Time_Traveled;
  double velocity;
  enum DircWall
   {
      BACK,
      FRONT
   };
  void SetWall(PhotonOut::DircWall w){ dircwall = w; }
  PhotonOut::DircWall GetWall() const { return dircwall; }
  PhotonOut::DircWall WallFromPhi(double const _phi){
    static double pi = TMath::Pi();
    if (
      ((_phi > pi/2.) && (_phi < 3.*pi/2)) ||
      ((_phi < -pi/2.) && (_phi > -3.*pi/2)) )
      return PhotonOut::BACK;

      else return PhotonOut::FRONT;
  }

  private:

  PhotonOut::DircWall dircwall;

  ClassDef(PhotonOut, 1);

};



class Photon : public PhotonOut
{
public:
  Photon(double theta=0., double phi=0.) : WhichParticle(0), Distance_Traveled(0.), Flag(0), Reflections(0),
  X_Distance(0.), Y_Distance(0.), Z_Distance(0.), Distance(0.), X_Time(0.), Y_Time(0.), Z_Time(0.), Time(0.),
  UnitVector( sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)) { Theta = theta; Phi=phi; }
  ~Photon(){}

  void SetAngle(double const t, double const p){ Theta = t; Phi = p; UpdateUnitVector(); }
  void SetCoordinates(double const x, double const y, double const z) { X=x; Y=y; Z=z; }
  void UpdateUnitVector(){
    UnitVector.SetXYZ( sin(Theta)*cos(Phi), sin(Theta)*sin(Phi), cos(Theta));
  }
  Photon To_Photon(PhotonOut p){ return Photon(p.Theta, p.Phi); }
  PhotonOut To_PhotonOut(Photon p){ return PhotonOut(p.Theta, p.Phi); }


  int WhichParticle;
  int Which;
  double Distance_Traveled;
  int Flag;
  int Reflections;
  int Wall;
  double X_Distance;
  double Y_Distance;
  double Z_Distance;
  double Distance;
  double X_Time;
  double Y_Time;
  double Z_Time;
  double Time;
  int EmissionGroup;
	double coord[3];
	double Traveled;


  TVector3 UnitVector;

  ClassDef(Photon, 1);
};

// ostream& operator<< (ostream& os, const Photon& p){
//   os << p.Theta << ", " << p.Phi;
//   return os;
// }

class PhotonEvent : public TObject
{
public:
  PhotonEvent() : Phase("Generation"), Focus(0), Event(0), NumberofPhotons(0) {}
  ~PhotonEvent(){}

  std::vector<Photon> Photons;
  std::string Phase;
  int Focus;
  int iterator;
  int Event;
  int NumberofPhotons;

  ClassDef(PhotonEvent, 1);
};




//===================================================================
//_______________________Photon Classes_______________________________
class Photon_new : public TObject
{
public:
	Photon_new() {}
	~Photon_new(){}

	double Theta;
	double Phi;

	ClassDef(Photon_new, 1);
};

class sPhoton : public Photon_new
{
public:
	sPhoton() : Traveled(0.) {}
	~sPhoton(){}

	double coord[3];
	double Traveled;

	ClassDef(sPhoton, 1);

};


#endif