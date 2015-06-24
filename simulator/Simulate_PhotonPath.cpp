#include <iostream>
#include <string>
#include <math.h>
#include "dirc_objects.h"
#include "simulator.h"
#include "../headers/functions.h"

using namespace std;
double pi = 3.14159265358979312;

/*================================================================================================
Simulate the trajectories for a single Photon of a single Event
================================================================================================*/
bool checkFinalAngle(double const& theta_out, double const& phi_out, double const& theta_in, double const& phi_in);
bool withinDiff(double const& v1, double const& v2, double diff);
void Simulate_PhotonPath(Detector& d, Photon &photon, double smear, bool print)
{
	static double original_theta; original_theta = photon.Theta;
	static double original_phi; original_phi = photon.Phi;
	print = false; // TEMPORARY
	if (print){ TabToLevel(2); cout << "Simulate_PhotonPath:\n"; }
	static Simulate simPho(0, 0);
	simPho.ResetTime();
	simPho.SetAngle(photon.Theta, photon.Phi);
	simPho.SetStart(photon.X, photon.Y, photon.Z);
	simPho.SetDim(d.Length, d.Width, d.Height);
	static double photon_velocity = 1/d.n;
	simPho.SetVelocity(photon_velocity);
	photon.SetVelocity(photon_velocity);

	Random r;
	while ((photon.X != d.Length) && (photon.X != 0))
	{
		simPho.GotoWall(print);
		double &x_p = simPho.coord[0];
		if( simPho.wall == 1 )
		{
			if ( withinDiff(x_p, 0., .01) ) photon.SetWall(Photon::BACK);
			if ( withinDiff(x_p, d.Length, .01) ) photon.SetWall(Photon::FRONT);

			if (photon.GetWall() != photon.WallFromPhi(photon.Phi)){
				cout << "photon wall = " << photon.GetWall() << " with phi = " << photon.Phi << " and subsequent wall " << photon.WallFromPhi(photon.Phi) << endl;
				exit(1);
			}
			photon.SetAngle(simPho.Theta, simPho.Phi);
			photon.X = simPho.coord[0];
			photon.Y = simPho.coord[1];
			photon.Z = simPho.coord[2];
			photon.Time_Traveled += simPho.GetTimeTraveled();
			static bool angle_passed;
			angle_passed = checkFinalAngle(photon.Theta, photon.Phi, original_theta, original_phi);
			if (!angle_passed){
				cout << "\noriginal: " << original_theta << ", " << original_phi << endl;
				cout << "current: " << photon << '\n';
				exit(1);
			}
			return;
		}

		double th = simPho.Theta;
		double ph = simPho.Phi;
		Photon pho_temp(th, ph);
		pho_temp.Wall = simPho.wall;
		// cout << pho_temp.Wall << ": " << pho_temp << endl;
		CheckAngel(d, pho_temp, "no");
		photon.Flag = pho_temp.Flag;
		if (photon.Flag == 1){
			return;
		}
		// Change_Angle(photon, Output.Trivial);
		simPho.Reflect(print);
		photon.Reflections++;
		// if (photon.Reflections > 800)
		// {
		// 	photon.Flag = 1;
		// 	return;
		// }
	}
}

bool checkFinalAngle(double const& theta_out, double const& phi_out, double const& theta_in, double const& phi_in){

	static double theta_reflect;
	static double phi_reflect;

	theta_reflect = TMath::Pi() - theta_in;
	phi_reflect = -phi_in;

	bool theta_pass = ( withinDiff(theta_out, theta_in, .01) || withinDiff(theta_out, theta_reflect, .01) );
	bool phi_pass = (withinDiff(phi_out, phi_in, .01) || withinDiff(phi_out, phi_reflect, .01));

	if (theta_pass && phi_pass)
		return true;
	else return false;

}

bool withinDiff(double const& v1, double const& v2, double diff){

	static double v1_v2;
	v1_v2 = fabs(v1-v2);
	if (v1_v2 <= diff) return true;
	else return false;
}