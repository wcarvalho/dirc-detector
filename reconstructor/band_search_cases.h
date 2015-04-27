// #include <cv.hpp>
// #include <highgui.h>
#include "Rounder.h"
#include <math.h>
#include "reconstructor.h"
#include <unordered_map>
#include <TSpectrum.h>
#include "Rotater.h"

double pi=3.14159265358979312;

bool isVectorForward(double const phi){
	if ( (phi > pi/2)  && (phi < 3*pi/2) )
		return false;
	else
		return true;
}


double distance_traveled(double theta, double phi, double x){

	static double xy = 0.;
	static double xyz = 0.;
	xy = sqrt(x*x*(TMath::Tan(phi)*TMath::Tan(phi) + 1));
	xyz = xy/TMath::Sin(theta);

	return xyz;
}


void findTimeBand(double& min_time, double& max_time, double const& emission_angle, ParticleOut const& particle){

	static Detector d;

	static Rotater r; r.Feed_Particle(particle.Theta, particle.Phi);
	r.ChangeFrame();


	int N = 100;
	static vector<double> phis; phis.resize(N,0.);

	static double target = 0.;
	static double delta_x = 0.;
	if (isVectorForward(particle.Phi)){
		target = 0.;
		delta_x = d.Length - particle.X;
	}
	else{
		target = pi;
		delta_x = particle.X;
	}

	static double target_difference = 0.;
	static double min_target_difference = 0.;
	static double closest_theta;
	static double closest_phi;
	static double current_theta;
	min_target_difference = 1.e10;

	for (unsigned i = 0; i < N; ++i){
		double& phi = phis.at(i);
		phi = i*((double)2*pi/N);
		current_theta = emission_angle;
		r.Rotate_Photon(current_theta, phi);

		target_difference = fabs(phi - target);
		if (target_difference < min_target_difference){
			min_target_difference = target_difference;
			closest_theta = current_theta;
			closest_phi = phi;
		}

	}

	static double photon_distance = 0.;
	photon_distance = distance_traveled(closest_theta, closest_phi, delta_x);

	static double shortest_time = 0.;
	shortest_time = photon_distance*1.e-2/3;

	min_time = shortest_time - .02;
	max_time = shortest_time + .08;
}
