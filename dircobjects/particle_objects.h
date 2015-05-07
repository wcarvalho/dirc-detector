#ifndef __PARTICLE_OBJECTS__
#define __PARTICLE_OBJECTS__

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include "TObject.h"
#include <map>
#include "TVector3.h"
#include "TH2D.h"
#include "dirc_objects.h"

class ParticleOut : public TObject
{
public:
	ParticleOut(double theta=1., double phi=0.) : Theta(theta), Phi(phi), Eta(0.), pt(0.), Time_Traveled(0.) { PossibleMasses(); }
	~ParticleOut(){}


	void PossibleMasses();
	double CalculateBeta(double mass);
	double CalculateBeta(double mass) const{
		return CalculateBeta(mass);
	};
	std::map <std::string, double > MassMap();
	std::map <std::string, double > EmissionAngleMap();
	std::map <std::string, double > EmissionAngleMap() const{
		return EmissionAngleMap();
	};
	virtual double CalculateMomentum(double m){
		double Beta = CalculateBeta(m);
		return sqrt((m*Beta)*(m*Beta)/(1-Beta*Beta));
	}
	virtual double CalculateMomentum(double m) const {
		return CalculateMomentum(m);
	}

	double Theta;
	double Phi;
	double X;
	double Y;
	double Eta;
	double pt;
	double p[3];
	double masses[5];
	double PhotonsPercm;
	double Time_Traveled;
	int Charge;
	int NumberofPhotons;

	std::vector<std::string> deftypes;
	std::vector<std::string> types;

  ClassDef(ParticleOut, 1);
};

class Particle : public ParticleOut
{
public:
  Particle(double theta=0., double phi=0.) : ConeAngle(.8),
  X_Distance(0.), Y_Distance(0.), Z_Distance(0.), Distance(0.), X_Time(0.), Y_Time(0.), Z_Time(0.), Time(0.), Emissions(4),
  UnitVector( sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta)), Beta(.8), name(""), nPhotonsPassed(0) { Theta = theta; Phi = phi; }
  ~Particle(){}

  double CalculatePhotonsPercm(double xlow, double xhi, double n, double z = 1., double alpha = 1./137);
	double CalculateMomentum() const;
	double CalculateMomentum();

	std::string GetName(){ return name; }

	Particle To_Particle(ParticleOut p1){ return Particle(p1.Theta, p1.Phi); }
	ParticleOut To_ParticleOut(Particle p1){ return ParticleOut(p1.Theta, p1.Phi); }

	double m;
  unsigned int Which;
  double Z;
  double IncidentX;
  double IncidentY;
  double ConeAngle;
  double X_Distance;
  double Y_Distance;
  double Z_Distance;
  double Distance;
  double X_Time;
  double Y_Time;
  double Z_Time;
  double Time;
  int Wall;
  int Emissions;
  TVector3 UnitVector;
  double Beta;
  std::string name;
  int nPhotonsPassed;
  double momentum;

  ClassDef(Particle, 1);
};

class ParticleEvent : public TObject
{
public:
  ParticleEvent() : Focus(0), Event(0), input(0.) {}
  ~ParticleEvent(){}

  std::vector<Particle> Particles;
  int Focus;
  int Event;
  int NumberofParticles;
  double input;

  ClassDef(ParticleEvent, 1);
};





//=============================================================================================
//						Particle Classes
//=============================================================================================
class Particle_new : public TObject
{
public:
	Particle_new() {}
	~Particle_new(){}

	double p[3]; //
	double Theta; //
	double Phi; //
	double X;
	double Y;
	int Charge; //

	ClassDef(Particle_new, 1);
};

class sParticle : public Particle_new
{
public:
	sParticle() : Beta(.8), NumberofPhotons(1000) {}
	~sParticle(){}

	double m; //
	double v; // don't need
	double Beta; //
	double EmissionAngle;
	int NumberofPhotons;

	ClassDef(sParticle, 1);

};


#endif