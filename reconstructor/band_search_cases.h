#include "Simulate.h"
#include "reconstructor.h"

int sign(double const& number);

void TallestBinContent(ParticleOut & particle, TH2D const& h, double const& smear, double const& width, double& center_min, double &center_max, vec_pair const&expected_photons, bool const& print){
	TH1D* h1 = h.ProjectionY();

	for (unsigned i = 0; i < h1->GetNbinsX(); ++i){
		double bin_center = h1->GetBinCenter(i);
		if ((bin_center > 1.) || (bin_center < .3)){
			h1->SetBinContent(i, 0);
		}
	}

	int max_bin = h1->GetMaximumBin();
	double center = h1->GetBinCenter(max_bin);
	center_min = center - width;
	center_max = center + width;
}
void x_distance(ParticleOut& P, double& x_back, double& x_forward){


	static Detector d;
	static Simulate sim(0.,0.);

	double& l = d.Length;
	double& w = d.Width;
	double& h = d.Height;

	sim.SetAngle(P.Theta, P.Phi);
	sim.SetDim(l, w, h);
	sim.SetStart(P.X, P.Y, 0.);
	sim.DistancetoWalls();
	sim.WhichWall();

	// static string name = "path length (meters)";
	// static TH1D H(name.c_str(), name.c_str(), 100, 0., .02);
	// H.Fill(sim.WillTravel()/100.);
	// H.SaveAs("path_length.root");
	sim.TravelDistance(sim.WillTravel());

	static double x_start;
	static double x_final;
	static double x_displacement;
	static double half_x_traveled;
	static double x_half_movement;

	x_start = P.X;
	x_final = sim.coord[0];
	x_displacement = x_final-x_start;
	half_x_traveled = fabs(x_displacement)/2.;

	x_half_movement = x_start + sign(x_displacement)*half_x_traveled;

	// cout << "x_start = " << x_start << endl;
	// cout << "x_final = " << x_final << endl;
	// cout << "x_displacement = " << x_displacement << endl;
	// cout << "half_x_traveled = " << half_x_traveled << endl;
	// cout << "x_half_movement = " << x_start << " + " << sign(x_displacement) << "*"<< half_x_traveled << endl;

	x_back = x_half_movement;
	// cout << "x_half_movement = " << x_half_movement << endl;
	x_forward = l - x_half_movement;
	// cout << "x_forward = " << x_forward << endl;

	return;
}
int sign(double const& number){
	if (number < 0) return -1;
	else return 1;
}