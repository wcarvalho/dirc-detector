#include "Simulate.h"

bool within_expectedphoton_threshold(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{
	const vector<double> &areasigmas = R.delSigArea;
	return (fabs(areasigmas[rec_i]) < threshold);
}
bool inside_box(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{

	const vector<double> &areasigmas = R.delSigArea;
	const vector<double> &thetasigmas = R.delSigTheta;
	return ( (fabs(areasigmas[rec_i]) < threshold) && (fabs(thetasigmas[rec_i]) < threshold) );
}
bool inside_circle(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold)
{

	const vector<double> &areasigmas = R.delSigArea;
	const vector<double> &thetasigmas = R.delSigTheta;
	static double square = 0.;
	square = areasigmas[rec_i]*areasigmas[rec_i] + thetasigmas[rec_i]*thetasigmas[rec_i];
	return ( sqrt(square) < 2*sqrt(threshold));
}

bool inside_box_xyplane(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold){
	bool in_xyplane = false;
	// FIXME: hardcoded d. have this as read me. can boost detector in or boost in values
	static Detector d;
	Simulate SimPar(P.Theta, P.Phi);
	SimPar.SetDim(d.Length, d.Width, d.Height);
	SimPar.SetStart(P.X, P.Y, 0);
	SimPar.DistancetoWalls();
	SimPar.WhichWall();
	int wall = SimPar.wall;
	if (wall == 3) {
		bool in_box = inside_box(P, R, rec_i, threshold);
		if (in_box) return true;
		else return false;
	}
	else return false;
}

//FIXME: boost inside_ellipse so that it
// bool inside_ellipse(const Particle& P, const TrackRecon& R, const int& rec_i, const double& threshold, const double& x1, const double& x2, const double& y1, const double& y2, const double& b)
// {
// 	static bool inside_ellipse;
// 	InsideEllipse(x1, y1, x2, y2, b, thetasigmas[rec_i], areasigmas[rec_i], inside_ellipse);
// 	return inside_ellipse;
// }

