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

	static string name = "path length (meters)";
	static TH1D H(name.c_str(), name.c_str(), 100, 0., .02);
	H.Fill(sim.WillTravel()/100.);
	H.SaveAs("path_length.root");
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

	x_back = x_half_movement;
	x_forward = l - x_half_movement;

	return;
}

double time_of_flight(ParticleOut& P){
	static double tp_sum;
	static double beta;
	auto masses = P.MassMap();

	tp_sum = 0;
	for (auto i = masses.begin(); i != masses.end(); ++i){
		double& mass = i->second;
		beta = P.CalculateBeta(mass);
		tp_sum += P.path_to_dirc()/beta;
	}

	double tp = tp_sum/masses.size();

	return std::move(tp);
}



void findTimeBand(double& time_min, double& time_max, ParticleOut& P){

	time_min = 0;
	time_max = 10;
	if (time_max < time_min){
		cout << "ERROR! time max < time min!\n\nExiting!!\n"; exit(1);
	}
}

int sign(double const& number){
	if (number < 0) return -1;
	else return 1;
}